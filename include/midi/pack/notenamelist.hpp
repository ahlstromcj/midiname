#if ! defined MIDINAME_MIDI_PACK_NOTENAMELIST_HPP
#define MIDINAME_MIDI_PACK_NOTENAMELIST_HPP

/*
 *  This file is part of midiname.
 *
 *  midiname is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  midiname is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with midiname; if not, write to the Free Software Foundation, Inc., 59
 *  Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * \file          notenamelist.hpp
 *
 *  Provides data structures mirroring some of the classes in the
 *  midnam_entities module.
 *
 * \library       midiname library
 * \author        Chris Ahlstrom
 * \date          2026-03-26
 * \updates       2026-03-28
 * \version       $Revision$
 *
 *  This module contains the midi::pack classes related to notenamelistes.
 *  No XML code is involved; no pointers are involved.
 *
 *  There are two flavors of NoteNameList:
 *
 *      -   A NoteNameList enclosing a number of Note elements. A
 *          ChannelNameSet can contain a UsesNoteNameList item.
 *          See tests/data/XLN_Audio_Addictive_Drums.midnam.
 *      -   A NoteNameList enclosing a number of NoteGroup elements,
 *          each containing a few Note elements. A Patch can refer
 *          to a given NoteNameList via a UsesNoteNameList (a
 *          simple string). See tests/data/Yamaha_EX_5_7.midnam.
 *
 *  Multiple NoteNameLists can appear in a patch-file. They seem to be
 *  mostly used for drum-notes and related groups of drum-notes.
 *
 *          MIDNAM Element          midi::pack class
 *
 *      -   Note                    note
 *      -   NoteGroup               notegroup
 *      -   NoteNameList            notenamelist
 */

#include <cstdint>                      /* std::uint16_t, uint8_t           */
#include <string>                       /* std::string<>                    */
#include <vector>                       /* std::vector instead of list<>    */

namespace midi
{

namespace pack
{

/**
 *  Note. Specifies the name and note number of the note.
 */

class note
{

public:

    /**
     *  A handy type, similar to the same alias in the control class.
     */

    using number = std::uint8_t;

    /**
     *  Provides a container for notes that can be used by NoteNameList
     *  and NoteGroup.
     */

    using list = std::vector<note>;

private:

    /**
     *  Note number.
     */

    number m_note_number { 0 };

    /**
     *  Note name.
     */

    std::string m_note_name { };

public:

    note () = default;

    note (int nnumber, const std::string & nname) :
        m_note_number   (number(nnumber)),
        m_note_name     (nname)
    {
        // no code
    }

    note (const note & id) = default;
    note & operator = (const note & id) = default;
    note (note && id) = default;
    note & operator = (note && id) = default;
    ~note () = default;

    number note_number () const
    {
        return m_note_number;
    };

    void note_number (int n)
    {
        m_note_number = number(n);
    }

    const std::string & note_name () const
    {
        return m_note_name;
    }

    void note_name (const std::string & name)
    {
        m_note_name = name;
    }

};          // class note

class notegroup
{

private:

    /**
     *  The name of the note-group.
     */

    std::string m_name;

    /**
     *  The list of the notes that the group contains.
     */

    note::list m_note_list;

public:

    notegroup () = default;

    notegroup (const std::string & gname) : m_name (gname)
    {
        // no code
    }

    notegroup (const notegroup & id) = default;
    notegroup & operator = (const notegroup & id) = default;
    notegroup (notegroup && id) = default;
    notegroup & operator = (notegroup && id) = default;
    ~notegroup () = default;

    const std::string & name () const
    {
        return m_name;
    }

    void name (const std::string & n)
    {
        m_name = n;
    }

    bool add_note (int num, const std::string & nname)
    {
        bool result { ! nname.empty() };
        if (result)
        {
            note n(num, nname);
            m_note_list.push_back(n);
        }
        return result;
    }

    note::list & note_list ()
    {
        return m_note_list;
    }

    const note::list & note_list () const
    {
        return m_note_list;
    }

    void note_list (const note::list & nl)
    {
        m_note_list = nl;
    }

};          // class notegroup

/**
 *  NoteNameList. This element either holds Notes directly, or it holds
 *  NoteGroups which themselves hold a few Notes. It is used by
 *  MasterDeviceNames, which can hold more than one Note.
 *
 *  It is also used indirectly (i.e. by name) in the Patch elements.
 */

class notenamelist
{

public:

    using list = std::vector<notenamelist>;

private:

    /**
     *  The "Name" notenamelist of the "<Value>" item.
     */

    std::string m_name { };

public:

    notenamelist () = default;

    notenamelist (const std::string & nname) : m_name (nname)
    {
        // no code
    }

    notenamelist (const notenamelist & id) = default;
    notenamelist & operator = (const notenamelist & id) = default;
    notenamelist (notenamelist && id) = default;
    notenamelist & operator = (notenamelist && id) = default;
    ~notenamelist () = default;

    const std::string & name () const
    {
        return m_name;
    }

    void set_name (const std::string & name)
    {
        m_name = name;
    }

};          // class notenamelist

}           // namespace pack

}           // namespace midi

#endif      // MIDINAME_MIDI_PACK_NOTENAMELIST_HPP

/*
 * notenamelist.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
