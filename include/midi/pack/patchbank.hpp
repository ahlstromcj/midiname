#if ! defined MIDINAME_MIDI_PACK_PATCHBANK_HPP
#define MIDINAME_MIDI_PACK_PATCHBANK_HPP

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
 */

/**
 * \file          patchbank.hpp
 *
 *  Provides data structures mirroring some of the classes in the
 *  midnam_entities module.
 *
 * \library       midiname library
 * \author        Chris Ahlstrom
 * \date          2026-03-15
 * \updates       2026-03-27
 * \version       $Revision$
 *
 *  This module contains the midi::pack classes related to patches.
 *  No XML code is involved; no pointers are involved.
 *
 * Notes on data types:
 *
 *      -   CDATA. This is character data represented by a string. It
 *          can contain spaces. It is used often for the human-readable
 *          name of an entity, enclosed in quotes. In our diagrams, we
 *          call it simply "string."
 *      -   NMTOKEN. This is a string that can contain letters, digits,
 *          and characters in the set { .-_: }. The most common use is
 *          to represent numbers as a quoted string. For numbers, in our
 *          diagrams, we call it "stringint".
 *      -   Boolean values are "false" or "true", which we call
 *          "stringbool".
 *
 *  This module defines the following classes, which are all ultimately
 *  contained indirectly by the patchbank class, as shown in the
 *  midnam-structure.dia diagram. The exception is MIDICommands,
 *  which gets its own midi::pack module.
 *
 *      -   programchange
 *      -   patchmidicommands
 *      -   patch
 *          -   patch::key
 *          -   patch::list
 *          -   patch::usesnotenamelist (a string)
 *      -   patchbank
 *          -   patchbank::usepatchnamelist (a string)
 *      -   patchbanks
 */

#include <cstdint>                      /* std::uint16_t, uint8_t           */
#include <list>                         /* std::list<>                      */
#include <map>                          /* std::map<>                       */
#include <string>                       /* std::string<>                    */
#include <vector>                       /* std::vector<>                    */

#include "cpp_types.hpp"                /* lib66::tokenization vector       */
#include "midi/pack/controlchange.hpp"  /* midi::pack::controlchange        */
#include "midi/pack/midicommands.hpp"   /* midi::pack::midicommands         */

namespace midi
{

namespace pack
{

/**
 *  ProgramChange. A small object using in conjunction with PatchMIDICommands.
 *  It provides a value needed to change to a different patch.
 *
 *  This attribute is (usually?) enclosed in a PatchMIDICommands item.
 *  This name is used as an attribute in a Patch entity.
 *  It is defined in MIDIEvents.dtd.
 */

class programchange
{

private:

    /**
     *  Attribute: "Number"
     *
     *  The patch number. It's stored here as an integer, but it is an
     *  8-bit quantity (std::uint8_t).
     */

    int m_prog_number { 0 };

    /**
     *  Indicates whether or not the patch containing this class
     *  is actually using a programchange ("<ProgramChange>") element.
     */

    bool m_is_active { false };

public:

    programchange () = default;

    programchange (int programno) :
        m_prog_number (programno),
        m_is_active (true)
    {
        // no code
    }

    programchange (const programchange & id) = default;
    programchange & operator = (const programchange & id) = default;
    programchange (programchange && id) = default;
    programchange & operator = (programchange && id) = default;
    ~programchange () = default;

    int prog_number () const
    {
        return m_prog_number;
    }

    void prog_number (int pn)
    {
        m_prog_number = pn;
    }

    bool is_active () const
    {
        return m_is_active;
    }

    void is_active (bool f)
    {
        m_is_active = f;
    }

};          // class programchange

/**
 *  This element (PatchMIDICommands) is included, when used, in a Patch
 *  element.
 */

class patchmidicommands
{

private:

    /**
     *  Item: "ControlChange", one or two items, so far as we have seen.
     *  Using a vector for flexibility.
     */

    controlchange::list m_control_changes { };

    /**
     *  Item: "ProgramChange", one item, so far as we have seen.
     */

    programchange m_program_change { };

public:

    patchmidicommands () = default;
    patchmidicommands (const patchmidicommands & id) = default;
    patchmidicommands & operator = (const patchmidicommands & id) = default;
    patchmidicommands (patchmidicommands && id) = default;
    patchmidicommands & operator = (patchmidicommands && id) = default;
    ~patchmidicommands () = default;

    controlchange::list & control_changes ()
    {
        return m_control_changes;
    }

    const controlchange::list & control_changes () const
    {
        return m_control_changes;
    }

    bool has_control_changes () const
    {
        return ! m_control_changes.empty();
    }

    programchange & program_change ()
    {
        return m_program_change;
    }

    const programchange & program_change () const
    {
        return m_program_change;
    }

    bool has_program_change () const
    {
        return m_program_change.is_active();
    }

};          // class patchmidicommands

/**
 *  Patch
 */

class patch
{

public:

    /**
     *  PatchNameList (see the midi_entities module) provides a list of
     *  pointers, not patch-names. The std::list container provides constant
     *  time insertion/removal of elements, but not fast random access.
     *
     *  Here, we hold the object, not a pointer.
     */

    using namelist = std::list<patch>;

    /**
     *  Provides specific types for bank and patch numbers.
     */

    using banknumber = std::uint16_t;
    using prognumber = std::uint8_t;

    /**
     *  Constants as specified by the MIDI specification.
     */

    static const banknumber c_bank_max      { 16383 };  /* 128x128-1, re 0  */
    static const prognumber c_program_max   {   127 };  /* 128-1, re 0      */

    /**
     *  Provides a way to order or look up patches. Also holds these
     *  values in each patch.
     */

    class key
    {

    private:

        banknumber m_bank       { 0 };
        prognumber m_program    { 0 };

    public:

        key () = default;
        key (int programno, int bankno = 0);
        key (const key & id) = default;
        key & operator = (const key & id) = default;
        key (key && id) = default;
        key & operator = (key && id) = default;
        ~key () = default;

        bool operator == (const key & id) const;
        bool operator < (const key & id) const;

        void set_bank (int bank);
        void set_program (int program);

        banknumber bank () const
        {
            return m_bank;
        }

        prognumber program () const
        {
            return m_program;
        }

    };          // nested class key

private:

    /**
     *  The "Name" value of the "<Patch>" item.
     */

    std::string m_name { };

    /**
     *  Some patches refer to a UsesNoteNameList. This is the name of
     *  that list, if it is not empty.
     */

    std::string m_note_name_list { };

    /**
     *  Contains the bank and program numbers used to sort the patches.
     *
     *  The patch number is the "Number" value of the "<Patch>" item.
     *  The "ProgramChange" value seems to be the same as the "Number".
     *  It is not always present; and in some cases the "Number" is
     *  3 digits ending in a plus sign (see Roland_SC_88_Pro.midnam).
     *  What's up with that? Are the two states correlated?
     */

    key m_id { };

public:

    patch () = default;
    patch
    (
        const std::string & pname,
        prognumber programno    = 0,
        banknumber bankno       = 0
    );
    patch (const patch & id) = default;
    patch & operator = (const patch & id) = default;
    patch (patch && id) = default;
    patch & operator = (patch && id) = default;
    ~patch () = default;

    const std::string & name () const
    {
        return m_name;
    }

    void set_name (const std::string & name)
    {
        m_name = name;
    }

    const std::string & note_name_list () const
    {
        return m_note_name_list;
    }

    void note_name_list (const std::string & nnl)
    {
        m_note_name_list = nnl;
    }

    prognumber program () const
    {
        return m_id.program();
    }

    void set_program (prognumber n)
    {
        m_id.set_program(n);
    }

    banknumber bank () const
    {
        return m_id.bank();
    }

    void set_bank (banknumber n)
    {
        m_id.set_bank(n);
    }

    const key & patch_primary_key () const
    {
        return m_id;
    }

};          // class patch

/**
 *  The patchbank class contains values from a "<PatchBank>" element.
 *  Some PatchBanks have only a MIDICommands element, while others have
 *  a PatchNameList.
 */

class patchbank
{

public:

    using list = std::vector<patchbank>;

private:

    /**
     *  The human-readable name of the patchbank.
     */

    std::string m_name { };

    /**
     *  The number of the PatchBank. It does *not* have a number according
     *  to the DTD, but the Ardour implementation supplies one. We can
     *  use it for ordering, perhaps. There does not seem to be uint16_t
     *  template specialization in <limits>, so we use UINT1t_MAX.
     */

    patch::banknumber m_number { UINT16_MAX };  /* some don't have a Number */

#if defined USE_PATCHBANK_PATCH_NAMELIST

    /**
     *  Some PatchBanks have only a MIDICommands element, while others have
     *  a PatchNameList, and others have a UsesPatchNameList.
     *
     *  We don't think we need to store this structure, as many other
     *  patchbanks might use it. Could make it a reference, but for now
     *  just use the name to do a lookup.
     */

    patch::namelist m_patch_name_list { };      /* PatchNameList            */

#endif

    /**
     *  If non-empty, this patchbank uses a patch::namelist.
     */

    std::string m_patch_list_name { };

    midicommands m_commands { };

    /**
     *  Indicates if the PatchBank has a "ROM" value of true versus false.
     */

    bool m_has_rom { false };

public:

    patchbank () = default;
    patchbank
    (
        const std::string & name,
        bool hasrom,
        patch::banknumber n = UINT16_MAX
    );
    patchbank (const patchbank & id) = default;
    patchbank & operator = (const patchbank & id) = default;
    patchbank (patchbank && id) = default;
    patchbank & operator = (patchbank && id) = default;
    ~patchbank () = default;

    const std::string & name () const
    {
        return m_name;
    }

    void set_name (const std::string & name)
    {
        m_name = name;
    }

    int bank_number () const
    {
        return int(m_number);
    }

    bool has_bank_number () const
    {
        return m_number != UINT16_MAX;
    }

#if defined USE_PATCHBANK_PATCH_NAMELIST

    const patch::namelist & patch_list_name () const
    {
        return m_patch_list_name;
    }

    int set_patch_name_list (const patch::namelist &);

#endif

    /**
     *  The alternative is having a midicommand object.
     */

    bool has_patch_name_list () const
    {
        return ! m_patch_list_name.empty();
    }

    const std::string & patch_name_list () const
    {
        return m_patch_list_name;
    }

    midicommands & commands ()
    {
        return m_commands;
    }

    const midicommands & commands () const
    {
        return m_commands;
    }

    bool has_rom () const
    {
        return m_has_rom;
    }

};          // class patchbank

/**
 *  The patchbanks class contains PatchBank elements.
 */

class patchbanks
{

private:

    patchbank::list m_patch_banks { };

public:

    patchbanks () = default;
    patchbanks (const patchbanks & id) = default;
    patchbanks & operator = (const patchbanks & id) = default;
    patchbanks (patchbanks && id) = default;
    patchbanks & operator = (patchbanks && id) = default;
    ~patchbanks () = default;

    patchbank::list & patch_banks ()
    {
        return m_patch_banks;
    }

    const patchbank::list & patch_banks () const
    {
        return m_patch_banks;
    }

    int count () const
    {
        return int(patch_banks().size());
    }

    void add (const patchbank & pb);
    lib66::tokenization names () const;

private:

};          // class patchbanks

}           // namespace pack

}           // namespace midi

#endif      // MIDINAME_MIDI_PACK_PATCHBANK_HPP

/*
 * patchbank.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
