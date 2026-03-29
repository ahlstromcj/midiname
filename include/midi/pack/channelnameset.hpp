#if ! defined MIDINAME_MIDI_PACK_CHANNELNAMESET_HPP
#define MIDINAME_MIDI_PACK_CHANNELNAMESET_HPP

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
 * \file          channelnameset.hpp
 *
 *  Provides data structures mirroring some of the classes in the
 *  midnam_entities module.
 *
 * \library       midiname library
 * \author        Chris Ahlstrom
 * \date          2026-03-25
 * \updates       2026-03-29
 * \version       $Revision$
 *
 *  This module defines the midi::pack::channelnameset class and some related
 *  classes.
 *
 *          MIDNAM Element          midi::pack class
 *
 *      -   ChannelNameSet          channelnameset
 *      -   AvailableChannel        availablechannel
 *      -   AvailableForChannels    availablechannel::list
 *      -   UsesControlNameList     usescontrolnamelist (a string)
 *      -   UsesNoteNameList        usesnotenamelist (a string)
 */

#include "midi/pack/patchbank.hpp"      /* midi::pack::patchbank(s) etc */

namespace midi
{

namespace pack
{

/**
 *  AvailableChannel. Encodes what channels are available from the device
 *  represented by the *.midnam file.
 *
 *  This class also defines Available channels as availablechannel::list
 *  for use in the channelset class.
 */

class availablechannel
{

public:

    using list = std::vector<availablechannel>;

private:

    int m_channel { };

    bool m_available { false };

public:

    availablechannel () = default;

    availablechannel (int channel, bool available) :
        m_channel   (channel),
        m_available (available)
    {
        // no code
    }

    availablechannel (const availablechannel & id) = default;
    availablechannel & operator = (const availablechannel & id) = default;
    availablechannel (availablechannel && id) = default;
    availablechannel & operator = (availablechannel && id) = default;
    ~availablechannel () = default;

    int channel () const
    {
        return m_channel;
    }

    void channel (int c)
    {
        m_channel = c;
    }

    bool available () const
    {
        return m_available;
    }

    void available (bool f)
    {
        m_available = f;
    }

};          // class availablechannel

/**
 *  ChannelNameSet.
 */

class channelnameset
{

public:

    /**
     *  Provides a list of ChannelNameSets.
     */

    using list = std::vector<channelnameset>;

private:

    /**
     *  Attribute: "Name"
     */

    std::string m_name { };

    /**
     *  See the green elements in midnam-structure.dia/png. Note that
     *  midi::pack::patchbanks is not in that diagram; it is
     *  an extra class to manage patchbanks.
     */

    patchbanks m_nameset_patchbanks { };

    /**
     *  A vector of AvailableChannels.
     */

    availablechannel::list m_available_channels { };

    /**
     *  When present, there seems to be only one control name list.
     */

    std::string m_control_name_list { };

    /**
     *  If non-empty, this channelnameset references a UsesNoteNameList.
     */

    std::string m_note_name_list { };

public:

    channelnameset () = default;

    channelnameset
    (
        const std::string & name,
        const std::string & ctrllist = "",
        const std::string & notelist = ""
    ) :
        m_name              (name),
        m_control_name_list (ctrllist),
        m_note_name_list    (notelist)
    {
        // no code
    }

    channelnameset (const channelnameset & id) = default;
    channelnameset & operator = (const channelnameset & id) = default;
    channelnameset (channelnameset && id) = default;
    channelnameset & operator = (channelnameset && id) = default;
    ~channelnameset () = default;

    const std::string & name () const
    {
        return m_name;
    }

    void name (const std::string & n)
    {
        m_name = n;
    }

    patchbanks & nameset_patchbanks ()
    {
        return m_nameset_patchbanks;
    }

    const patchbanks & nameset_patchbanks () const
    {
        return m_nameset_patchbanks;
    }

    void nameset_patchbanks (const patchbanks & pb)
    {
        m_nameset_patchbanks = pb;
    }

    availablechannel::list & available_channels ()
    {
        return m_available_channels;
    }

    const availablechannel::list & available_channels () const
    {
        return m_available_channels;
    }

    void available_channels (const availablechannel::list & acl)
    {
        m_available_channels = acl;
    }

    const std::string & control_name_list () const
    {
        return m_control_name_list;
    }

    void control_name_list (const std::string & cnl)
    {
        m_control_name_list = cnl;
    }

    const std::string & note_name_list () const
    {
        return m_note_name_list;
    }

    void note_name_list (const std::string & nnl)
    {
        m_note_name_list = nnl;
    }

};          // class channelnameset

}           // namespace pack

}           // namespace midi

#endif      // MIDINAME_MIDI_PACK_CHANNELNAMESET_HPP

/*
 * channelnameset.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
