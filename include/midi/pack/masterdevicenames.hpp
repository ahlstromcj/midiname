#if ! defined MIDINAME_MIDI_PACK_MASTERDEVICENAMES_HPP
#define MIDINAME_MIDI_PACK_MASTERDEVICENAMES_HPP

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
 * \file          masterdevicenames.hpp
 *
 *  Provides data structures mirroring some of the classes in the
 *  midnam_entities module.
 *
 * \library       midiname library
 * \author        Chris Ahlstrom
 * \date          2026-03-25
 * \updates       2026-03-27
 * \version       $Revision$
 *
 *  This module contains the midi::pack classes related to
 *  masterdevicenameses.
 *
 *  No XML code is involved; no pointers are involved.
 *
 *          MIDNAM Element          midi::pack class
 *
 *      -   MasterDeviceNames       masterdevicenames
 */

#include <string>                       /* std::string<>                    */
#include <vector>                       /* std::vector<>                    */

#include "midi/pack/channelnameset.hpp" /* midi::pack::channelnameset       */
#include "midi/pack/models.hpp"         /* midi::pack::models & model       */
#include "midi/pack/notenamelist.hpp"   /* midi::pack::notenamelist, note   */

namespace midi
{

namespace pack
{

/**
 *  MasterDeviceNames. This element organizes manufacturer/model information,
 *  plus ChannelNameSets and NoteNameLists.
 */

class masterdevicenames
{

private:

    /**
     *  This class holds the Manufacturer name and the list of
     *  Models (model names).
     */

    models m_model_information;

    /**
     *  This class holds classes related to patches and patchbanks.
     *  There can be 0, 1, or many name-sets.
     */

    channelnameset::list m_channel_name_sets;

    /**
     *  This class manages NoteGroups and Notes.
     */

    notenamelist::list m_note_name_list;

public:

    masterdevicenames () = default;
    masterdevicenames (const masterdevicenames & id) = default;
    masterdevicenames & operator = (const masterdevicenames & id) = default;
    masterdevicenames (masterdevicenames && id) = default;
    masterdevicenames & operator = (masterdevicenames && id) = default;
    ~masterdevicenames () = default;

    const std::string & manufacturer () const
    {
        return m_model_information.manufacturer();
    }

    void manufacturer (const std::string & m)
    {
        m_model_information.manufacturer(m);
    }

    models & model_information ()
    {
        return m_model_information;
    }

    const models & model_information () const
    {
        return m_model_information;
    }

    channelnameset::list & channel_name_sets ()
    {
        return m_channel_name_sets;
    }

    const channelnameset::list & channel_name_sets () const
    {
        return m_channel_name_sets;
    }

    notenamelist::list & note_name_list ()
    {
        return m_note_name_list;
    }

    const notenamelist::list & note_name_list () const
    {
        return m_note_name_list;
    }

};          // class masterdevicenames

/*--------------------------------------------------------------------------
 *  Free functions.
 *--------------------------------------------------------------------------*/

}           // namespace pack

}           // namespace midi

#endif      // MIDINAME_MIDI_PACK_MASTERDEVICENAMES_HPP

/*
 * masterdevicenames.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
