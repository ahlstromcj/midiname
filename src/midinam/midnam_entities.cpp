/*
 *  This file is part of midiname.
 *
 *  midiname is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  midiname is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with midiname; if not, write to the Free Software Foundation, Inc., 59
 *  Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Copyright (C) 2008-2012 Hans Baier <hansfbaier@googlemail.com>
 * Copyright (C) 2008-2016 David Robillard <d@drobilla.net>
 * Copyright (C) 2008-2017 Paul Davis <paul@linuxaudiosystems.com>
 * Copyright (C) 2009-2012 Carl Hetherington <carl@carlh.net>
 * Copyright (C) 2013-2019 Robin Gareus <robin@gareus.org>
 * Copyright (C) 2015-2016 Tim Mayberry <mojofunk@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/**
 * \file          midnam_entities.cpp
 *
 *    Provides the implementations for safe replacements for the various
 *    XML functions useful in handling MIDINAM files.
 *
 * \library       midiname library
 * \author        Chris Ahlstrom
 * \date          2026-02-20
 * \updates       2026-03-06
 * \version       $Revision$
 *
 */

#include "exceptions.hpp"               /* lib66::failed_construction class */
#include "midnam_entities.hpp"
#include "util/strfunctions.hpp"

namespace midi
{

namespace nam
{

/**
 *  Free functions.
 */

std::uint16_t
set_uint16 (int bank)
{
    return std::max(0, std::min(bank, int(c_midi_bank_max)));
}

std::uint8_t
set_uint8 (int program)
{
    return std::max(0, std::min(program, int(c_midi_program_max)));
}

/**
 *  Static functions.
 */

static int
s_string_to_int (const xml66::XMLTree & tree, const std::string & str)
{
    if (str.empty())                    //  || *endptr != '\0')
    {
        std::string msg
        {
            util::string_format
            (
                "%s: Bad number '%s'", V(tree.filename()), V(str)
            )
        };
        std::cerr << msg << std::endl;
        return (-1);
    }
    else
    {
        int i { util::string_to_int(str, (-1)) };
        return i;
    }
}

static int
initialize_primary_key_from_commands
(
    const xml66::XMLTree & tree,
    PatchPrimaryKey & id,                           // output parameter
    const xml66::XMLNode * node
)
{
    uint16_t bank    { 0 };
    uint8_t  program { 0 };
    const xml66::XMLNodeList events { node->children() };
    for (auto node : events)
    {
        if (node->name() == "ControlChange")
        {
            const std::string & control { node->property("Control")->value() };
            const std::string & value   { node->property("Value")->value() };
            if (control == "0")
                bank |= s_string_to_int(tree, value) << 7;
            else if (control == "32")
                bank |= s_string_to_int(tree, value);
        }
        else if (node->name() == "ProgramChange")
        {
            const std::string & num { node->property("Number")->value() };
            if (num.empty())                        // assert(number != "");
                break;

            program = s_string_to_int(tree, num);
        }
    }
    id = PatchPrimaryKey(program, bank);
    return 0;
}

/**
 * Class: Patch.
 */

Patch::Patch
(
    const std::string & name,
    uint8_t programno,
    uint16_t bankno
) :
    m_name  (name),
    m_id    (programno, bankno)
{
    // no code
}

xml66::XMLNode &
Patch::get_state () const
{
    xml66::XMLNode * node = new xml66::XMLNode("Patch");

    /*
     * XXX this is totally wrong
     */

    node->set_property("Number", m_id.program());
    node->set_property("Name",   m_name);

#if 0
    typedef std::list<std::shared_ptr<Evoral::Event<Temporal::Beats>>>
        PatchMidiCommands;

    xml66::XMLNode * commands = node->add_child("PatchMIDICommands");
    for
    (
        PatchMidiCommands::const_iterator event = _patch_midi_commands.begin();
        event != _patch_midi_commands.end();
        ++event
    )
    {
        commands->add_child_copy(Evoral::MIDIXML::midi_to_xml(*event));
    }
#endif

    return *node;
}

int
Patch::set_state
(
    const xml66::XMLTree & tree,
    const xml66::XMLNode & node
)
{
    if (node.name() != "Patch")
    {
        std::cerr
            << "Incorrect node type '" << node.name()
            << "' handed to patch" << " contents "
            << node.content() <<
            std::endl
            ;
        return (-1);
    }

    /*
     * Note there is a "Number" attribute, but it's really more like a label
     * and is often not numeric.  We currently do not use it.
     */

    const xml66::XMLProperty * program_change
    {
        node.property("ProgramChange")
    };
    if (not_nullptr(program_change))
    {
        m_id = PatchPrimaryKey
        (
            s_string_to_int(tree, program_change->value()), m_id.bank()
        );
    }

    const xml66::XMLProperty * name { node.property("Name") };
    if (is_nullptr(name))
        return (-1);

    m_name = name->value();

    xml66::XMLNode * commands { node.child("PatchMIDICommands") };
    if (not_nullptr(commands))
    {
        if
        (
            initialize_primary_key_from_commands(tree, m_id, commands) &&
                is_nullptr(program_change)
        )
        {
            return (-1);        /* failed to find a program number anywhere */
        }
    }

    xml66::XMLNode * use_note_name_list { node.child("UsesNoteNameList") };
    if (not_nullptr(use_note_name_list))
    {
        m_note_list_name = use_note_name_list->property("Name")->value();
    }
    return 0;
}

/**
 * Class: PatchBank.
 */

xml66::XMLNode &
PatchBank::get_state () const
{
    xml66::XMLNode * node { new xml66::XMLNode("PatchBank") };
    node->set_property("Name", m_name);
    xml66::XMLNode * patch_name_list { node->add_child("PatchNameList") };
    for (auto p : m_patch_name_list)
    {
        patch_name_list->add_child_nocopy(p->get_state());
    }
    return *node;
}

int
PatchBank::set_state
(
    const xml66::XMLTree & tree,
    const xml66::XMLNode & node
)
{
    if (node.name() != "PatchBank")     // assert(node.name() == "PatchBank");
        return (-1);

    m_name = node.property("Name")->value();

    xml66::XMLNode * commands { node.child("MIDICommands") };
    if (not_nullptr(commands))
    {
        PatchPrimaryKey id { 0, 0 };
        if (initialize_primary_key_from_commands(tree, id, commands))
            return (-1);

        m_number = id.bank();
    }

    xml66::XMLNode * patch_name_list { node.child("PatchNameList") };
    if (not_nullptr(patch_name_list))
    {
        const xml66::XMLNodeList patches { patch_name_list->children() };
        for (auto p : patches)
        {
            PatchPtr patch(new Patch(std::string(), 0, m_number));
            if (patch->set_state(tree, *(p)) == 0)
                m_patch_name_list.push_back(patch);
        }
    }
    else
    {
        xml66::XMLNode * use_patch_name_list
        {
            node.child ("UsesPatchNameList")
        };
        if (not_nullptr(use_patch_name_list))
        {
            m_patch_list_name = use_patch_name_list->property("Name")->value();
        }
        else
        {
            std::cerr
                << "Patch without patch name list; patchfile ignored"
                << std::endl
                ;
            return (-1);
        }
    }
    return 0;
}

int
PatchBank::set_patch_name_list (const PatchNameList & pnl)
{
    m_patch_name_list = pnl;
    m_patch_list_name.clear();
    for (auto p : m_patch_name_list)
        p->set_bank_number(m_number);

    return 0;
}

std::ostream &
operator << (std::ostream & os, const ChannelNameSet & cns)
{
    os
        << "ChannelNameSet: name = " << cns.m_name << std::endl
        << "Map size " << cns.m_patch_map.size() << std::endl
        << "List size " << cns.m_patch_list.size() << std::endl
        << "Patch list name = [" << cns.m_patch_list_name << ']' << std::endl
        << "Available channels : "
        ;
    for (const auto a : cns.m_available_for_channels)
        os << int(a) << ' ';

    os << std::endl;
    for (auto pb : cns.m_patch_banks)
    {
        os
            << "\tPatch bank " << (pb)->name()
            << " with " << (pb)->patch_name_list().size()
            << " patches\n"
            ;
        for (auto pni : pb->patch_name_list())
        {
            os
                << "\t\tPatch name " << pni->name()
                << " prog " << (int) pni->program_number()
                << " bank " << pni->bank_number()
                << std::endl
                ;
        }
    }
    return os;
}

/**
 * Class: ChannelNameSet.
 */

void
ChannelNameSet::set_patch_banks (const /* ChannelNameSet:: */ PatchBanks & pb)
{
    m_patch_banks = pb;
    m_patch_map.clear();
    m_patch_list.clear();
    m_patch_list_name.clear();
    m_available_for_channels.clear();
    for (auto pbi : m_patch_banks)
    {
        for (auto pni : pbi->patch_name_list())
        {
            m_patch_map[pni->patch_primary_key()] = pni;
            m_patch_list.push_back(pni->patch_primary_key());
        }
    }
    for (uint8_t n = 0; n < 16; ++n)
        m_available_for_channels.insert(n);
}

PatchPtr
ChannelNameSet::previous_patch (const PatchPrimaryKey & key)
{
    for
    (
        PatchList::const_iterator i = m_patch_list.begin();
        i != m_patch_list.end(); ++i
    )
    {
        if (*i == key)
        {
            if (i != m_patch_list.begin())
            {
                --i;
                return m_patch_map[*i];
            }
        }
    }
    return PatchPtr();
}

PatchPtr
ChannelNameSet::next_patch (const PatchPrimaryKey & key)
{
    for
    (
        PatchList::const_iterator i = m_patch_list.begin();
        i != m_patch_list.end(); ++i
    )
    {
        if (*i == key)
        {
            if (++i != m_patch_list.end())
                return m_patch_map[*i];
            else
                --i;
        }
    }
    return PatchPtr();
}

void
ChannelNameSet::use_patch_name_list (const PatchNameList & pnl)
{
    for (auto p : pnl)
    {
        const PatchPrimaryKey & k { p->patch_primary_key() };
        m_patch_map[k] = p;
        m_patch_list.push_back(k);
    }
}

xml66::XMLNode &
ChannelNameSet::get_state () const
{
    xml66::XMLNode * node { new xml66::XMLNode("ChannelNameSet") };
    node->set_property("Name", m_name);

    xml66::XMLNode * available { node->add_child("AvailableForChannels") };
    if (not_nullptr(available))                 // assert(available);
    {
        for (uint8_t channel = 0; channel < 16; ++channel)
        {
            xml66::XMLNode * availablechannel
            {
                available->add_child("AvailableChannel")
            };
            if (not_nullptr(availablechannel))  // assert(availablechannel);
            {
                availablechannel->set_property("Channel", channel);

                bool found
                {
                    m_available_for_channels.find(channel) !=
                        m_available_for_channels.end()
                };
                availablechannel->set_property
                (
                    "Available", found ? "true" : "false"
                );
            }
        }
        for (auto patchbank : m_patch_banks)
            node->add_child_nocopy(patchbank->get_state());
    }
    return *node;
}

int
ChannelNameSet::set_state
(
    const xml66::XMLTree & tree,
    const xml66::XMLNode & node
)
{
    if (node.name() == "ChannelNameSet")        // assert(node.name() == ...)
    {
        m_name = node.property("Name")->value();

        const xml66::XMLNodeList children { node.children() };
        for (auto node : children)
        {
            if (is_nullptr(node))               // assert(node);
                break;

            if (node->name() == "AvailableForChannels")
            {
                /*
                 * xml_shared_nodes channels; xml66::XMLSharedNodeList channels
                 */

                xml66::SharedNodeListPtr channels
                {
                    tree.find
                    (
                        "//AvailableChannel[@Available = 'true']/@Channel",
                        node
                    )
                };
                for (auto c : *channels)
                {
                    try
                    {
                        (void) m_available_for_channels.insert
                        (
                            s_string_to_int(tree, c->attribute_value())
                        );
                    }
                    catch (xml66::XMLException & e)
                    {
                        std::cerr
                            << "ChannelNameSet::set_state: "
                            << e.what ()
                            << std::endl
                            ;
                    }
                }
            }
            else if (node->name() == "PatchBank")
            {
                PatchBankPtr bank(new PatchBank());
                bank->set_state(tree, *node);
                m_patch_banks.push_back(bank);

                const PatchNameList & patches { bank->patch_name_list() };
                for (auto p : patches)
                {
                    const PatchPrimaryKey & k { p->patch_primary_key() };
                    m_patch_map[k] = p;
                    m_patch_list.push_back(k);
                }
            }
            else if (node->name() == "UsesNoteNameList")
            {
                m_note_list_name = node->property ("Name")->value();
            }
            else if (node->name() == "UsesControlNameList")
            {
                m_control_list_name = node->property ("Name")->value();
            }
        }
    }
    return 0;
}

/**
 * Class: Note.
 */

xml66::XMLNode &
Note::get_state () const
{
    xml66::XMLNode * node { new xml66::XMLNode("Note") };
    node->set_property("Number", m_number);
    node->set_property("Name",   m_name);
    return *node;
}

int
Note::set_state (const xml66::XMLTree & tree, const xml66::XMLNode & node)
{
    if (node.name() == "Note")                  // assert(node.name() == ...)
    {
        int num { s_string_to_int(tree, node.property("Number")->value()) };
        if (num > 127)
        {
            std::string msg
            {
                util::string_format
                (
                    "%s: Note number %d (%s) out of range",
                    V(tree.filename()), num, V(m_name)
                )
            };
            std::cerr << msg << std::endl;      // PBD::warning
            return (-1);
        }
        m_number = num;
        m_name   = node.property("Name")->value();
        return 0;
    }
    else
        return (-1);
}

xml66::XMLNode &
NoteNameList::get_state () const
{
    xml66::XMLNode * node { new xml66::XMLNode("NoteNameList") };
    node->set_property("Name", m_name);
    return *node;
}

/**
 *  Static function.
 */

static void
s_add_note_from_xml
(
    /* NoteNameList:: */
    Notes & notes,
    const xml66::XMLTree & tree,
    const xml66::XMLNode & node
)
{
    NotePtr note(new Note());
    if (! note->set_state(tree, node))
    {
        if (! notes[note->number()])
        {
            notes[note->number()] = note;
        }
        else
        {
            std::string msg
            {
                util::string_format
                (
                    "%s: Duplicate note number %d (%s) ignored",
                    V(tree.filename()), int(note->number()), V(note->name())
                )
            };
            std::cerr << msg << std::endl;          // PBD::warning
        }
    }
}

int
NoteNameList::set_state
(
    const xml66::XMLTree & tree,
    const xml66::XMLNode & node
)
{
    if (node.name() == "NoteNameList")              // assert(node.name() == ...)
    {
        m_name = node.property("Name")->value();
        m_notes.clear();
        m_notes.resize(128);
        for (auto i : node.children())
        {
            if (i->name() == "Note")
            {
                s_add_note_from_xml(m_notes, tree, *i);
            }
            else if (i->name() == "NoteGroup")
            {
                for (auto j : i->children())
                {
                    if (j->name() == "Note")
                    {
                        s_add_note_from_xml(m_notes, tree, *j);
                    }
                    else
                    {
                        std::string msg
                        {
                            util::string_format
                            (
                                "%s: Invalid NoteGroup child %s ignored",
                                V(tree.filename()), V(j->name())
                            )
                        };
                        std::cerr << msg << std::endl;  // PBD::warning
                    }
                }
            }
        }
        return 0;
    }
    else
        return (-1);
}

/**
 * Class: Value
 */

xml66::XMLNode &
Value::get_state () const
{
    xml66::XMLNode * node { new xml66::XMLNode("Value") };
    node->set_property("Number", m_number);
    node->set_property("Name",   m_name);
    return *node;
}

int
Value::set_state (const xml66::XMLTree & tree, const xml66::XMLNode & node)
{
    if (node.name() == "Value")                 // assert(node.name() == ...)
    {
        m_number = s_string_to_int(tree, node.property("Number")->value());
        m_name   = node.property("Name")->value();
        return 0;
    }
    else
        return (-1);
}

/**
 * Class: ValueNameList
 */

xml66::XMLNode &
ValueNameList::get_state () const
{
    xml66::XMLNode * node { new xml66::XMLNode("ValueNameList") };
    node->set_property("Name", m_name);
    return *node;
}

int
ValueNameList::set_state
(
    const xml66::XMLTree & tree,
    const xml66::XMLNode & node
)
{
    if (node.name() == "ValueNameList")         // assert(node.name() == ...)
    {
        const xml66::XMLProperty * name_prop = node.property("Name");
        if (not_nullptr(name_prop))
        {
            /*
             * May be anonymous if written inline within a single
             * <Control> tag.
             */

            m_name = name_prop->value();
        }
        m_values.clear();
        for (auto i : node.children())
        {
            if (i->name() == "Value")
            {
                ValuePtr value(new Value());
                value->set_state (tree, *i);
                if (m_values.find(value->number()) == m_values.end())
                {
                    m_values.insert(make_pair(value->number(), value));
                }
                else
                {
                    std::string msg
                    {
                        util::string_format
                        (
                            "%s: Duplicate value %d ignored",
                            V(tree.filename()), value->number()
                        )
                    };
                    std::cerr << msg << std::endl;          // PBD::warning
                }
            }
        }
        return 0;
    }
    else
        return (-1);
}

ValueConstPtr
ValueNameList::value (uint16_t num) const
{
    Values::const_iterator i { m_values.find(num) };
    if (i != m_values.end())
        return i->second;

    return ValueConstPtr();
}

ValueConstPtr
ValueNameList::max_value_below (uint16_t num) const
{
    Values::const_iterator i { m_values.lower_bound(num) };
    if (i == m_values.end())
        return ValueConstPtr();

    if (i->first == num)
    {
        return i->second;                           /* Exact match          */
    }
    else if (i == m_values.begin())
    {
        return ValueConstPtr();                     /* No value is < num    */
    }
    else
    {
        /*
         * Found the smallest element >= num, so the previous one
         * is our result.
         */

        --i;
        return i->second;
    }
}

/**
 * Class: Control
 */

xml66::XMLNode &
Control::get_state () const
{
    xml66::XMLNode * node { new xml66::XMLNode("Control") };
    node->set_property("Type",   m_type);
    node->set_property("Number", m_number);
    node->set_property("Name",   m_name);
    return *node;
}

int
Control::set_state (const xml66::XMLTree & tree, const xml66::XMLNode & node)
{
    if (node.name() == "Control")               // assert(node.name() == ...)
    {
        if (node.property("Type"))
            m_type = node.property("Type")->value();
        else
            m_type = "7bit";

        if (m_type == "NRPN")
            return (-1);

        m_number = s_string_to_int(tree, node.property("Number")->value());
        m_name   = node.property("Name")->value();
        for (auto i : node.children())
        {
            if (i->name() == "Values")
            {
                /*
                 * <Values> has Min and Max properties, but we don't care
                 * about them.
                 */

                for (auto j : i->children())
                {
                    if (j->name() == "ValueNameList")
                    {
                        m_value_name_list =
                            ValueNameListPtr(new ValueNameList());

                        m_value_name_list->set_state(tree, *j);
                    }
                    else if (j->name() == "UsesValueNameList")
                    {
                        m_value_name_list_name =
                            j->property("Name")->value();
                    }
                }
            }
        }
        return 0;
    }
    else
        return (-1);
}

/**
 * Class: ControlNameList
 */

xml66::XMLNode &
ControlNameList::get_state () const
{
    xml66::XMLNode * node { new xml66::XMLNode("ControlNameList") };
    node->set_property("Name", m_name);
    return *node;
}

int
ControlNameList::set_state
(
    const xml66::XMLTree & tree,
    const xml66::XMLNode & node
)
{
    if (node.name() == "ControlNameList")       // assert(node.name() == ...)
    {
        m_name = node.property("Name")->value();
        m_controls.clear();
        for (auto i : node.children())
        {
            if (i->name() == "Control")
            {
                ControlPtr control(new Control());
                if (control->set_state (tree, *i))
                    continue;

                if (m_controls.find(control->number()) == m_controls.end())
                {
                    m_controls.insert(make_pair(control->number(), control));
                }
                else
                {
                    std::string msg
                    {
                        util::string_format
                        (
                            "%s: Duplicate control %d ignored",
                            V(tree.filename()), control->number()
                        )
                    };
                    std::cerr << msg << std::endl;      // PBD::warning
                }
            }
        }
        return 0;
    }
    else
        return (-1);
}

ControlConstPtr
ControlNameList::control (uint16_t num) const
{
    Controls::const_iterator c { m_controls.find(num) };
    if (c != m_controls.end())
        return c->second;

    return ControlConstPtr();
}

/**
 * Class: CustomDeviceMode
 */

int
CustomDeviceMode::set_state
(
    const xml66::XMLTree & tree,
    const xml66::XMLNode & anode
)
{
    if (anode.name() == "CustomDeviceMode")         // assert(anode.name()...)
    {
        m_name = anode.property("Name")->value();

        SharedNodeListPtr channel_name_set_assignments
        {
            tree.find
            (
                "//ChannelNameSetAssign", const_cast<xml66::XMLNode *>(&anode)
            )
        };
        for (auto i : *channel_name_set_assignments)
        {
            int channel
            {
                s_string_to_int(tree, i->property("Channel")->value())
            };
            const std::string & name_set { i->property("NameSet")->value() };

            // assert(1 <= channel && channel <= 16 );

            if (channel >= 1 && channel <= 16 )     // assert(...)
                m_channel_name_set_assignments[channel - 1] = name_set;
        }
        return 0;
    }
    else
        return (-1);
}

xml66::XMLNode &
CustomDeviceMode::get_state () const
{
    xml66::XMLNode * custom_device_mode
    {
        new xml66::XMLNode("CustomDeviceMode")
    };
    custom_device_mode->set_property("Name", m_name);

    xml66::XMLNode * assignments
    {
        custom_device_mode->add_child("ChannelNameSetAssignments")
    };
    for
    (
        int i = 0; i < 15 && ! m_channel_name_set_assignments[i].empty(); ++i
    )
    {
        xml66::XMLNode * assign
        {
            assignments->add_child("ChannelNameSetAssign")
        };
        assign->set_property("Channel", i + 1);
        assign->set_property("NameSet", m_channel_name_set_assignments[i]);
    }
    return *custom_device_mode;
}

ValueNameListConstPtr
MasterDeviceNames::value_name_list_by_control
(
    const std::string & mode,
    uint8_t channel,
    uint8_t num
)
{
    ChannelNameSetPtr channames { channel_name_set_by_channel(mode, channel) };
    if (! channames)
        return ValueNameListConstPtr();

    ControlNameListPtr control_names
    {
        control_name_list(channames->control_list_name())
    };
    if (! control_names)
        return ValueNameListConstPtr();

    ControlConstPtr control { control_names->control(num) };
    if (! control)
        return ValueNameListConstPtr();

    if (! control->value_name_list_name().empty())
        return value_name_list(control->value_name_list_name());
    else
        return control->value_name_list();
}

CustomDeviceModePtr
MasterDeviceNames::custom_device_mode_by_name (const std::string & mode_name)
{
    return m_custom_device_modes[mode_name];
}

ChannelNameSetPtr
MasterDeviceNames::channel_name_set_by_channel
(
    const std::string & mode,
    uint8_t channel
)
{
    CustomDeviceModePtr cdm { custom_device_mode_by_name(mode) };
    ChannelNameSetPtr cns
    {
        m_channel_name_sets[cdm->channel_name_set_name_by_channel(channel)]
    };
    return cns;
}

PatchPtr
MasterDeviceNames::find_patch
(
    const std::string & mode,
    uint8_t channel,
    const PatchPrimaryKey & key
)
{
    ChannelNameSetPtr cns { channel_name_set_by_channel(mode, channel) };
    if (! cns)
        return PatchPtr();

    return cns->find_patch(key);
}

ChannelNameSetPtr
MasterDeviceNames::channel_name_set (const std::string & name)
{
    ChannelNameSets::const_iterator i { m_channel_name_sets.find(name) };
    if (i != m_channel_name_sets.end())
        return i->second;

    return ChannelNameSetPtr();
}

std::shared_ptr<ControlNameList>
MasterDeviceNames::control_name_list (const std::string & name)
{
    ControlNameLists::const_iterator i { m_control_name_lists.find(name) };
    if (i != m_control_name_lists.end())
        return i->second;

    return ControlNameListPtr();
}

ValueNameListPtr
MasterDeviceNames::value_name_list (const std::string & name)
{
    ValueNameLists::const_iterator i { m_value_name_lists.find(name) };
    if (i != m_value_name_lists.end())
        return i->second;

    return std::shared_ptr<ValueNameList>();
}

NoteNameListPtr
MasterDeviceNames::note_name_list (const std::string & name)
{
    NoteNameLists::const_iterator i { m_note_name_lists.find(name) };
    if (i != m_note_name_lists.end())
        return i->second;

    return NoteNameListPtr();
}

std::string
MasterDeviceNames::note_name
(
    const std::string & mode_name,
    uint8_t channel,
    uint16_t bank,
    uint8_t program,
    uint8_t num
)
{
    std::string result;
    if (num < 128)
    {
        NoteNameListConstPtr note_names;
        PatchConstPtr patch
        (
            find_patch(mode_name, channel, PatchPrimaryKey(program, bank))
        );
        if (patch)
            note_names = note_name_list(patch->note_list_name());

        if (! note_names)
        {
            /*
             * No note names specific to this patch, check the ChannelNameSet.
             */

            ChannelNameSetPtr channames
            {
                channel_name_set_by_channel(mode_name, channel)
            };
            if (channames)
                note_names = note_name_list(channames->note_list_name());
        }
        if (note_names)
        {
            NoteConstPtr note(note_names->notes()[num]);
            result = note ? note->name() : "" ;
        }
    }
    return result;
}

int
MasterDeviceNames::set_state (const xml66::XMLTree & tree, const xml66::XMLNode &)
{
    // Manufacturer

    SharedNodeListPtr manufacturer { tree.find("//Manufacturer") };
    if (manufacturer->size() == 1)              // assert(...)
        m_manufacturer = manufacturer->front()->children().front()->content();

    // Models

    SharedNodeListPtr models { tree.find("//Model") };
    if (models->size() >= 1)                    // assert(...)
    for (auto i : *models)
    {
        const xml66::XMLNodeList & contents { i->children() } ;
        if (contents.size() == 1)               // assert(...)
        {
            xml66::XMLNode * content { *(contents.begin()) };
            if (content->is_content())          // assert(...)
                m_models.insert(content->content());
        }
    }

    // CustomDeviceModes

    SharedNodeListPtr custom_device_modes { tree.find("//CustomDeviceMode") };
    for (auto i : *custom_device_modes)
    {
        CustomDeviceModePtr custom_device_mode(new CustomDeviceMode());
        custom_device_mode->set_state(tree, *i);
        m_custom_device_modes[custom_device_mode->name()] = custom_device_mode;
        m_custom_device_mode_names.push_back(custom_device_mode->name());
    }

    // ChannelNameSets

    SharedNodeListPtr channel_name_sets { tree.find("//ChannelNameSet") };
    for (auto i : *channel_name_sets)
    {
        ChannelNameSetPtr channel_name_set(new ChannelNameSet());
        channel_name_set->set_state(tree, *i);
        m_channel_name_sets[channel_name_set->name()] = channel_name_set;
    }

    // NoteNameLists

    SharedNodeListPtr note_name_lists { tree.find("//NoteNameList") };
    for (auto i : *note_name_lists)
    {
        NoteNameListPtr note_name_list(new NoteNameList());
        note_name_list->set_state(tree, *i);
        m_note_name_lists[note_name_list->name()] = note_name_list;
    }

    // ControlNameLists

    SharedNodeListPtr control_name_lists { tree.find("//ControlNameList") };
    for (auto i : *control_name_lists)
    {
        ControlNameListPtr control_name_list(new ControlNameList());
        control_name_list->set_state(tree, *i);
        m_control_name_lists[control_name_list->name()] = control_name_list;
    }

    // ValueNameLists

    SharedNodeListPtr value_name_lists
    {
        tree.find
        (
            "/child::MIDINameDocument/child::MasterDeviceNames/"
            "child::ValueNameList"
        )
    };
    for (auto i : *value_name_lists)
    {
        ValueNameListPtr value_name_list(new ValueNameList());
        value_name_list->set_state(tree, *i);
        m_value_name_lists[value_name_list->name()] = value_name_list;
    }

    // global/post-facto PatchNameLists

    SharedNodeListPtr patch_name_lists
    {
        tree.find
        (
            "/child::MIDINameDocument/child::MasterDeviceNames/"
            "child::PatchNameList"
        )
    };
    for (auto i : *patch_name_lists)
    {
        std::string n;
        i->get_property ("Name", n);

        PatchNameList patch_name_list;
        const xml66::XMLNodeList patches { i->children() };
        for (auto p : patches)
        {
            PatchPtr patch (new Patch ());
            if (patch->set_state(tree, *p) == 0)
                patch_name_list.push_back(patch);
        }
        if (! patch_name_list.empty())
        {
            m_patch_name_lists[i->property ("Name")->value()] =
                patch_name_list;
        }
    }

    /*
     * Now traverse patches and hook up anything that used UsePatchNameList
     * to the right patch list.
     */

    for (auto cns : m_channel_name_sets)
    {
        /* ChannelNameSet:: */ PatchBanks pbs { cns.second->patch_banks() };
        PatchNameLists::iterator p;
        for (auto pb : pbs)
        {
            const std::string & pln { pb->patch_list_name() };
            if (!pln.empty())
            {
                if
                (
                    (p = m_patch_name_lists.find(pln)) !=
                        m_patch_name_lists.end()
                )
                {
                    if (pb->set_patch_name_list(p->second))
                        return (-1);

                    cns.second->use_patch_name_list(p->second);
                }
                else
                {
                    std::string msg
                    {
                        util::string_format
                        (
                            "Patch list name %s not found - "
                            "patch file ignored", V(pln)
                        )
                    };
                    std::cerr << msg << std::endl;
                    return (-1);
                }
            }
        }
    }
    return 0;
}

xml66::XMLNode &
MasterDeviceNames::get_state () const
{
    static xml66::XMLNode nothing("<nothing>");
    return nothing;
}

/**
 * Class: MIDINameDocument
 */

MIDINameDocument::MIDINameDocument (const std::string & file_path) :
    m_file_path (file_path)
{
    xml66::XMLTree document;
    if (! document.read(file_path))
    {
        throw lib66::failed_constructor("MIDINameDocument");
    }
    document.set_filename(file_path);
    set_state(document, *document.root());
}

int
MIDINameDocument::set_state
(
    const xml66::XMLTree & tree,
    const xml66::XMLNode &
)
{
    // Author

    SharedNodeListPtr author { tree.find("//Author") };
    if (author->size() < 1)
    {
        std::cerr << "No author information in MIDNAM file" << std::endl;
        return (-1);
    }

    if (author->front()->children().size() > 0)
        m_author = author->front()->children().front()->content();

    // MasterDeviceNames

    SharedNodeListPtr master_device_names_list
    {
        tree.find("//MasterDeviceNames")
    };
    for (auto i : *master_device_names_list)
    {
        MasterDeviceNamesPtr master_device_names(new MasterDeviceNames());
        if (master_device_names->set_state(tree, *i))
            return (-1);

        for (auto model : master_device_names->models())
        {
            m_master_device_names_list.insert
            (
                std::pair<std::string, MasterDeviceNamesPtr>
                (
                    model, master_device_names
                )
            );
            m_all_models.insert(model);
        }
    }
    return 0;
}

xml66::XMLNode &
MIDINameDocument::get_state () const
{
    static xml66::XMLNode nothing("<nothing>");
    return nothing;
}

MasterDeviceNamesPtr
MIDINameDocument::master_device_names (const std::string & model)
{
    MasterDeviceNamesList::const_iterator m
    {
        m_master_device_names_list.find(model)
    };
    if (m != m_master_device_names_list.end())
        return MasterDeviceNamesPtr(m->second);

    return MasterDeviceNamesPtr();
}

}           // namespace nam

}           // namespace midi

/*
 * midnam_entities.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */

