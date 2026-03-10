#if ! defined MIDINAME_MIDI_NAM_MIDINAM_ENTITIES_HPP
#define MIDINAME_MIDI_NAM_MIDINAM_ENTITIES_HPP

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
 * Copyright (C) 2008-2014 David Robillard <d@drobilla.net>
 * Copyright (C) 2008-2015 Paul Davis <paul@linuxaudiosystems.com>
 * Copyright (C) 2009-2012 Carl Hetherington <carl@carlh.net>
 * Copyright (C) 2014-2018 Robin Gareus <robin@gareus.org>
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
 * \file          midnam_entities.hpp
 *
 *    Provides the implementations for safe replacements for the various
 *    XML functions useful in handling MIDINAM files.
 *
 * \library       midiname library
 * \author        Chris Ahlstrom
 * \date          2026-02-20
 * \updates       2026-03-09
 * \version       $Revision$
 *
 *  Classes:
 *
 *      ChannelNameSet
 *      Control
 *      ControlNameList
 *      CustomDeviceMode
 *      MIDINameDocument
 *      MasterDeviceNames
 *      Note
 *      NoteNameList
 *      Patch
 *      PatchBank
 *      PatchPrimaryKey
 *      Value
 *      ValueNameList
 *
 *  Missing:
 *
 *      ControlChange
 *      MIDICommands
 *      PatchMIDICommands
 *      PatchNameList
 */

#include <algorithm>                    /* std::min(), std::max()           */
#include <cstdint>                      /* std::uint16_t, uint8_t           */
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "xml/xml66xx.hpp"              /* XML classes in xml66 namespace   */

namespace midi
{

namespace nam
{

/*
 *  Summary of aliases in this module.
 */

/**
 *  Some more aliases.
 *
 * using PatchList             = std::list<PatchPrimaryKey>;
 * using PatchPtr              = std::shared_ptr<Patch>;
 * using PatchConstPtr         = std::shared_ptr<const Patch>;
 * using PatchNameList         = std::list<PatchPtr>;
 * using PatchNameLists        = std::map<std::string, PatchNameList>;
 * using PatchMap              = std::map<PatchPrimaryKey, PatchPtr>;
 * using PatchBankPtr          = std::shared_ptr<PatchBank>;
// using PatchBankNameList     = std::list<PatchBankPtr>;
 * using PatchBanks            = std::list<PatchBankPtr>;
 * using ChannelNameSetPtr     = std::shared_ptr<ChannelNameSet>;
 * using ChannelNameSets       = std::map<std::string, ChannelNameSetPtr>;
 * using NotePtr               = std::shared_ptr<Note>;
 * using NoteConstPtr          = std::shared_ptr<const Note>;
 * using Notes                 = std::vector<NotePtr>;
 * using NoteList              = std::vector<NotePtr>;
 * using NoteNameListPtr       = std::shared_ptr<NoteNameList>;
 * using NoteNameListConstPtr  = std::shared_ptr<const NoteNameList>;
 * using NoteNameLists         = std::map<std::string, NoteNameListPtr>;
 * using ValueConstPtr         = std::shared_ptr<const Value>;
 * using ValuePtr              = std::shared_ptr<Value>;
 * using Values                = std::map<std::uint16_t, ValuePtr>;
 * using ValueNameList_xxx     = std::list<ValuePtr>;
 * using ValueNameListPtr      = std::shared_ptr<ValueNameList>;
 * using ValueNameListConstPtr = std::shared_ptr<const ValueNameList>;
 * using ValueNameLists        = std::map<std::string, ValueNameListPtr>;
 * using ControlPtr            = std::shared_ptr<Control>;
 * using ControlConstPtr       = std::shared_ptr<const Control>;
 * using ControlList           = std::map<std::uint16_t, ControlPtr>;
 * using ControlNameListPtr    = std::shared_ptr<ControlNameList>;
 * using ControlNameLists      = std::map<std::string, ControlNameListPtr>;
 * using Controls              = std::map<std::uint16_t, ControlPtr>;
 * using CustomDeviceModeNames = std::list<std::string>;
 * using CustomDeviceModePtr   = std::shared_ptr<CustomDeviceMode>;
 * using CustomDeviceModes     = std::map<std::string, CustomDeviceModePtr>;
 * using MasterDeviceNamesPtr  = std::shared_ptr<MasterDeviceNames>;
 * using MasterDeviceNamesList = std::map<std::string, MasterDeviceNamesPtr>;
 */

using AvailableForChannels  = std::set<std::uint8_t>;
using Models                = std::set<std::string>;
using SharedNodeListPtr     = std::shared_ptr<xml66::XMLSharedNodeList>;

const std::uint16_t c_midi_bank_max     { 16383 };
const std::uint8_t c_midi_program_max   {   127 };

extern std::uint16_t set_uint16 (int bank);
extern std::uint8_t set_uint8 (int program);

class PatchPrimaryKey
{

private:

    std::uint16_t m_bank    { 0 };
    std::uint8_t  m_program { 0 };

public:

    PatchPrimaryKey (int programno = 0, int bankno = 0) :
        m_bank      (set_uint16(bankno)),
        m_program   (set_uint8(programno))
    {
        // no code
    }

    PatchPrimaryKey (const PatchPrimaryKey & id) = default;
    PatchPrimaryKey & operator = (const PatchPrimaryKey & id) = default;
    PatchPrimaryKey (PatchPrimaryKey && id) = default;
    PatchPrimaryKey & operator = (PatchPrimaryKey && id) = default;
    ~PatchPrimaryKey () = default;

    bool operator == (const PatchPrimaryKey & id) const
    {
        return m_bank == id.m_bank && m_program == id.m_program;
    }

    /*
     *  Strict weak ordering.
     */

    bool operator < (const PatchPrimaryKey & id) const
    {
        if (m_bank < id.m_bank)
            return true;
        else if (m_bank == id.m_bank && m_program < id.m_program)
            return true;

        return false;
    }

    void set_bank (int bank)
    {
        m_bank = set_uint16(bank);
    }

    void set_program (int program)
    {
        m_program = set_uint8(program);
    }

    std::uint16_t bank () const
    {
        return m_bank;
    }

    std::uint8_t program () const
    {
        return m_program;
    }

};          //struct PatchPrimaryKey

using PatchList = std::list<PatchPrimaryKey>;

/**
 *  Patch
 */

class Patch
{

public:

    // typedef std::list<std::shared_ptr<Patch> > PatchNameList;
    // using pointer = std::shared_ptr<Patch>;
    // using list = std::list<pointer>;

private:

    std::string m_name { };
    std::string m_note_list_name { };
    PatchPrimaryKey m_id { };

public:

    Patch
    (
        const std::string & pname   = "",
        std::uint8_t programno      = 0,
        std::uint16_t bankno        = 0
    );
    virtual ~Patch()
    {
        // no code
    };

    const std::string & name () const
    {
        return m_name;
    }

    void set_name (const std::string & name)
    {
        m_name = name;
    }

    const std::string & note_list_name() const
    {
        return m_note_list_name;
    }

    std::uint8_t program_number () const
    {
        return m_id.program();
    }

    void set_program_number (std::uint8_t n)
    {
        m_id.set_program(n);
    }

    std::uint16_t bank_number () const
    {
        return m_id.bank();
    }

    void set_bank_number (std::uint16_t n)
    {
        m_id.set_bank(n);
    }

    const PatchPrimaryKey & patch_primary_key () const
    {
        return m_id;
    }

    xml66::XMLNode & get_state () const;
    int set_state (const xml66::XMLTree &, const xml66::XMLNode &);

};

using PatchPtr          = std::shared_ptr<Patch>;
using PatchConstPtr     = std::shared_ptr<const Patch>;
using PatchNameList     = std::list<PatchPtr>;
using PatchNameLists    = std::map<std::string, PatchNameList>;
using PatchMap          = std::map<PatchPrimaryKey, PatchPtr>;

/**
 *  PatchBank
 */

class PatchBank
{

private:

    std::string     m_name { };
    std::uint16_t   m_number { 0 };
    PatchNameList   m_patch_name_list { };
    std::string     m_patch_list_name { };

public:

    PatchBank
    (
        std::uint16_t n = UINT16_MAX,
        const std::string & aname = ""
    ) :
        m_name      (aname),
        m_number    (n)
    {
        // no code
    };

    virtual ~PatchBank ()
    {
        // no code
    };

    const std::string & name () const
    {
        return m_name;
    }

    void set_name(const std::string & aname)
    {
        m_name = aname;
    }

    int number () const
    {
        return m_number;
    }

    const PatchNameList & patch_name_list () const
    {
        return m_patch_name_list;
    }

    const std::string & patch_list_name() const
    {
        return m_patch_list_name;
    }

    int set_patch_name_list (const PatchNameList &);
    xml66::XMLNode & get_state () const;
    int set_state (const xml66::XMLTree &, const xml66::XMLNode &);

};

using PatchBankPtr  = std::shared_ptr<PatchBank>;
using PatchBanks    = std::list<PatchBankPtr>;

// using PatchBankNameList     = std::list<PatchBankPtr>;

/**
 *  ChannelNameSet
 */

class ChannelNameSet
{
    friend std::ostream & operator << (std::ostream &, const ChannelNameSet &);

public:

    // using AvailableForChannels = std::set<uint8_t>;
    // using PatchBanks = std::list<std::shared_ptr<PatchBank>>;
    // using PatchMap = std::map<PatchPrimaryKey, std::shared_ptr<Patch>>;
    // using PatchList = std::list<PatchPrimaryKey>;

private:

    std::string          m_name { };
    AvailableForChannels m_available_for_channels { };
    PatchBanks           m_patch_banks { };
    PatchMap             m_patch_map { };
    PatchList            m_patch_list { };
    std::string          m_patch_list_name { };
    std::string          m_note_list_name { };
    std::string          m_control_list_name { };

public:

    ChannelNameSet() = default;
    ChannelNameSet (const std::string & name) : m_name (name)
    {
        // no code
    };

    virtual ~ChannelNameSet () = default;

    const std::string & name () const
    {
        return m_name;
    }

    void set_name (const std::string & name)
    {
        m_name = name;
    }

    const PatchBanks & patch_banks() const
    {
        return m_patch_banks;
    }

    bool available_for_channel (std::uint8_t channel) const
    {
        return m_available_for_channels.find(channel) !=
            m_available_for_channels.end();
    }

    PatchPtr find_patch (const PatchPrimaryKey & key)
    {
        return m_patch_map[key];    // THIS COULD ADD AN ENTRY
    }

    PatchPtr previous_patch (const PatchPrimaryKey & key);
    PatchPtr next_patch (const PatchPrimaryKey & key);

    const std::string & note_list_name () const
    {
        return m_note_list_name;
    }

    const std::string & control_list_name () const
    {
        return m_control_list_name;
    }

    xml66::XMLNode & get_state () const;
    int set_state (const xml66::XMLTree &, const xml66::XMLNode &);
    void set_patch_banks (const PatchBanks &);
    void use_patch_name_list (const PatchNameList &);

};

using ChannelNameSetPtr = std::shared_ptr<ChannelNameSet>;
using ChannelNameSets   = std::map<std::string, ChannelNameSetPtr>;

/* extern */ std::ostream & operator << (std::ostream &, const ChannelNameSet &);

/**
 *  Note
 */

class Note
{

public:

    // using pointer = std::shared_ptr<Note>;

private:

    std::uint8_t m_number   { 0 };
    std::string m_name      {   };

public:

    Note ()  = default;

    Note (std::uint8_t num, const std::string & name) :
        m_number    (num),
        m_name      (name)
    {
        // no code
    }

    const std::string & name () const
    {
        return m_name;
    }

    void set_name (const std::string & name)
    {
        m_name = name;
    }

    std::uint8_t number() const
    {
        return m_number;
    }

    void set_number (std::uint8_t num)
    {
        m_number = num;
    }

    xml66::XMLNode & get_state () const;
    int set_state (const xml66::XMLTree &, const xml66::XMLNode &);

};

using NotePtr       = std::shared_ptr<Note>;
using NoteConstPtr  = std::shared_ptr<const Note>;
using Notes         = std::vector<NotePtr>;
using NoteList      = std::vector<NotePtr>;

class NoteNameList
{

public:

    // typedef std::vector< std::shared_ptr<Note> > Notes;
    // using Notes = std::vector<Note::pointer> Notes;

private:

    std::string m_name { };
    Notes       m_notes { };

public:

    NoteNameList ()
    {
        m_notes.resize(128);
    }

    NoteNameList (const std::string & name) : m_name (name)
    {
        m_notes.resize(128);
    }

    const std::string & name () const
    {
        return m_name;
    }

    void set_name (const std::string & name)
    {
        m_name = name;
    }

    const Notes & notes () const
    {
        return m_notes;
    }

    xml66::XMLNode & get_state () const;
    int set_state (const xml66::XMLTree &, const xml66::XMLNode &);

};

using NoteNameListPtr       = std::shared_ptr<NoteNameList>;
using NoteNameListConstPtr  = std::shared_ptr<const NoteNameList>;
using NoteNameLists         = std::map<std::string, NoteNameListPtr>;

/**
 *  Value
 */

class Value
{

public:

    // using pointer = std::shared_ptr<Value>;
    // using const_pointer = std::shared_ptr<const Value>;

private:

    std::uint16_t m_number { 0 };
    std::string m_name { };

public:

    Value () = default;
    Value (std::uint16_t num, const std::string & name) :
        m_number    (num),
        m_name      (name)
    {
        // no code
    }

    std::uint16_t number () const
    {
        return m_number;
    }

    void set_number (std::uint16_t num)
    {
        m_number = num;
    }

    const std::string & name () const
    {
        return m_name;
    }

    void set_name(const std::string & name)
    {
        m_name = name;
    }

    xml66::XMLNode & get_state () const;
    int set_state (const xml66::XMLTree &, const xml66::XMLNode &);

};

using ValueConstPtr = std::shared_ptr<const Value>;
using ValuePtr      = std::shared_ptr<Value>;
using Values        = std::map<std::uint16_t, ValuePtr>;
// using ValueNameList = std::list<ValuePtr>;

/**
 * ValueNameList
 */

class ValueNameList
{

public:

    // using Values = std::map<uint16_t, Value::pointer>;
    // using pointer = std::shared_ptr<ValueNameList>;
    // using const_pointer = std::shared_ptr<const ValueNameList>;
    // using ValueNameListPtr = std::shared_ptr<ValueNameList>;

private:

    std::string m_name { };
    Values      m_values { };

public:

    ValueNameList () = default;

    ValueNameList (const std::string & name) : m_name (name)
    {
        // no code
    }

    const std::string & name () const
    {
        return m_name;
    }

    void set_name (const std::string & name)
    {
        m_name = name;
    }

    ValueConstPtr value (std::uint16_t num) const;
    ValueConstPtr max_value_below (std::uint16_t num) const;

    const Values & values () const
    {
        return m_values;
    }

    xml66::XMLNode & get_state () const;
    int set_state(const xml66::XMLTree &, const xml66::XMLNode &);

};

using ValueNameListPtr      = std::shared_ptr<ValueNameList>;
using ValueNameListConstPtr = std::shared_ptr<const ValueNameList>;
using ValueNameLists        = std::map<std::string, ValueNameListPtr>;

/**
 *  Control
 */

class Control
{
    // using pointer = std::shared_ptr<Control>;
    // using const_pointer = std::shared_ptr<const Control>;

private:

    std::string m_type { };
    std::uint16_t m_number { 0 };
    std::string m_name { };
    std::string m_value_name_list_name { };     // Global, UsesValueNameList
    ValueNameListPtr m_value_name_list { };     // Local, ValueNameList

public:

    Control() = default;

    Control
    (
        const std::string & type,
        const std::uint16_t num,
        const std::string & name
    ) :
        m_type      (type),
        m_number    (num),
        m_name      (name)
    {
        // no code
    }

    const std::string & type () const
    {
        return m_type;
    }

    std::uint16_t number () const
    {
        return m_number;
    }

    const std::string & name () const
    {
        return m_name;
    }

    const std::string & value_name_list_name () const
    {
        return m_value_name_list_name;
    }

    ValueNameListConstPtr value_name_list () const
    {
        return m_value_name_list;
    }

    void set_type (const std::string & type)
    {
        m_type = type;
    }

    void set_number (std::uint16_t num)
    {
        m_number = num;
    }

    void set_name (const std::string & name)
    {
        m_name = name;
    }

    xml66::XMLNode & get_state () const;
    int set_state(const xml66::XMLTree &, const xml66::XMLNode &);
};

using ControlPtr        = std::shared_ptr<Control>;
using ControlConstPtr   = std::shared_ptr<const Control>;
using ControlList       = std::map<std::uint16_t, ControlPtr>;
using Controls          = std::map<std::uint16_t, ControlPtr>;

/**
 *  ControlNameList
 */

class ControlNameList
{

public:

    // using Controls = std::map<uint16_t, ControlPtr>;

private:

    std::string m_name { };
    Controls    m_controls { };

public:

    ControlNameList () = default;

    ControlNameList (const std::string & name) : m_name (name)
    {
        // no code
    }

    const std::string & name () const
    {
        return m_name;
    }

    void set_name (const std::string & name)
    {
        m_name = name;
    }

    ControlConstPtr control (std::uint16_t num) const;

    const Controls & controls () const
    {
        return m_controls;
    }

    xml66::XMLNode & get_state () const;
    int set_state(const xml66::XMLTree &, const xml66::XMLNode &);
};

using ControlNameListPtr    = std::shared_ptr<ControlNameList>;
using ControlNameLists      = std::map<std::string, ControlNameListPtr>;

/**
 *  CustomDeviceMode
 */

class CustomDeviceMode
{

private:

    /**
     * array index = channel number
     * string contents = name of channel name set
     */

    std::string m_name { };
    std::string m_channel_name_set_assignments [16] { };

public:

    CustomDeviceMode () = default;
    virtual ~CustomDeviceMode() = default;

    const std::string & name () const
    {
        return m_name;
    }

    void set_name (const std::string & name)
    {
        m_name = name;
    }

    xml66::XMLNode & get_state () const;
    int set_state (const xml66::XMLTree &, const xml66::XMLNode &);

    /*
     * Note: channel here is 0-based while in the MIDNAM-file it's 1-based.
     */

    std::string channel_name_set_name_by_channel (std::uint8_t channel)
    {
        std::string result;
        if (channel <= 15)                      /* assert(channel <= 15);   */
            result = m_channel_name_set_assignments[channel];

        return result;
    }

};

using CustomDeviceModeNames = std::list<std::string>;
using CustomDeviceModePtr   = std::shared_ptr<CustomDeviceMode>;
using CustomDeviceModes     = std::map<std::string, CustomDeviceModePtr>;

/**
 *  MasterDeviceNames
 */

class MasterDeviceNames
{

private:

    std::string           m_manufacturer { };
    Models                m_models { };
    CustomDeviceModes     m_custom_device_modes { };
    CustomDeviceModeNames m_custom_device_mode_names { };
    ChannelNameSets       m_channel_name_sets { };
    NoteNameLists         m_note_name_lists { };
    PatchNameLists        m_patch_name_lists { };
    ControlNameLists      m_control_name_lists { };
    ValueNameLists        m_value_name_lists { };

public:

#if 0
    typedef std::set<std::string> Models;
    typedef std::map<std::string, std::shared_ptr<CustomDeviceMode>> CustomDeviceModes;
    typedef std::list<std::string> CustomDeviceModeNames;
    typedef std::map<std::string, std::shared_ptr<ChannelNameSet>> ChannelNameSets;
    typedef std::map<std::string, std::shared_ptr<NoteNameList>> NoteNameLists;
    typedef std::map<std::string, std::shared_ptr<ControlNameList>> ControlNameLists;
    typedef std::map<std::string, std::shared_ptr<ValueNameList>> ValueNameLists;
    typedef std::map<std::string, PatchNameList> PatchNameLists;
#endif

    MasterDeviceNames() = default;
    virtual ~MasterDeviceNames() = default;

    const std::string & manufacturer () const
    {
        return m_manufacturer;
    }

    void set_manufacturer (const std::string & manufacturer)
    {
        m_manufacturer = manufacturer;
    }

    const Models & models () const
    {
        return m_models;
    }

    void set_models (const Models & some_models)
    {
        m_models = some_models;
    }

    const ControlNameLists & controls () const
    {
        return m_control_name_lists;
    }

    const ValueNameLists & values () const
    {
        return m_value_name_lists;
    }

    ValueNameListConstPtr value_name_list_by_control
    (
        const std::string & mode,
        std::uint8_t channel,
        std::uint8_t num
    );

    const CustomDeviceModeNames & custom_device_mode_names() const
    {
        return m_custom_device_mode_names;
    }

    CustomDeviceModePtr custom_device_mode_by_name
    (
        const std::string & mode_name
    );
    ChannelNameSetPtr channel_name_set_by_channel
    (
        const std::string & mode,
        std::uint8_t channel
    );
    PatchPtr find_patch
    (
        const std::string & mode,
        std::uint8_t channel,
        const PatchPrimaryKey & key
    );

    ControlNameListPtr control_name_list (const std::string & name);
    ValueNameListPtr   value_name_list (const std::string & name);
    NoteNameListPtr    note_name_list (const std::string & name);
    ChannelNameSetPtr  channel_name_set (const std::string & name);
    std::string note_name
    (
        const std::string & mode_name,
        std::uint8_t channel,
        std::uint16_t bank,
        std::uint8_t program,
        std::uint8_t num
    );

    xml66::XMLNode & get_state () const;
    int set_state (const xml66::XMLTree &, const xml66::XMLNode &);

};

using MasterDeviceNamesPtr  = std::shared_ptr<MasterDeviceNames>;
using MasterDeviceNamesList = std::map<std::string, MasterDeviceNamesPtr>;

/**
 * MIDINameDocument
 */

class MIDINameDocument
{

public:

    // Maps Model names to MasterDeviceNames

#if 0
    typedef std::map<std::string, std::shared_ptr<MasterDeviceNames>>
        MasterDeviceNamesList;
#endif

private:

    std::string m_file_path { };
    std::string m_author { };
    MasterDeviceNamesList m_master_device_names_list { };
    /*MasterDeviceNames::*/ Models m_all_models { };

public:

    MIDINameDocument () = default;
    MIDINameDocument (const std::string & file_path);
    virtual ~MIDINameDocument () = default;

    const std::string & file_path () const
    {
        return m_file_path;
    }

    const std::string & author () const
    {
        return m_author;
    }

    void set_author (const std::string & author)
    {
        m_author = author;
    }

    void set_file_path (const std::string & file_path)
    {
        m_file_path = file_path;
    }

    MasterDeviceNamesPtr master_device_names (const std::string & model);

    const MasterDeviceNamesList & master_device_names_by_model () const
    {
        return m_master_device_names_list;
    }

    const /* MasterDeviceNames:: */ Models & all_models () const
    {
        return m_all_models;
    }

    xml66::XMLNode & get_state () const;
    int set_state (const xml66::XMLTree &, const xml66::XMLNode &);

};          // class MIDINameDocument

using MIDINameDocumentPtr = std::shared_ptr<MIDINameDocument>;

}           // namespace nam

}           // namespace midi

#endif      // MIDINAME_MIDI_NAM_MIDINAM_ENTITIES_HPP

/*
 * midi_midnam_entities.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
