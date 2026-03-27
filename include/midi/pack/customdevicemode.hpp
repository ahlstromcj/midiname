#if ! defined MIDINAME_MIDI_PACK_CUSTOMDEVICEMODE_HPP
#define MIDINAME_MIDI_PACK_CUSTOMDEVICEMODE_HPP

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
 * \file          customdevicemode.hpp
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
 *  This module contains the midi::pack classes related to customdevicemodees.
 *  No XML code is involved; no pointers are involved.
 *
 *          MIDNAM Element              midi::pack class
 *
 *      -   CustomDeviceMode            customdevicemode
 *      -   ChannelNameSetAssign        channelnamesetassign
 *      -   ChannelNameSetAssignments   channelnamesetassign::list
 *      -   DeviceModeEnable            devicemodeenable
 */

#include <string>                       /* std::string<>                    */
#include <vector>                       /* std::vector<>                    */

#include "midicommands.hpp"             /* midi::pack::midicommands         */

namespace midi
{

namespace pack
{

/**
 *  DeviceModeEnable contains, usually, it seems a SysEx MIDICommand, which
 *  contains a SysExDeviceID.
 */

class devicemodeenable
{

private:

    midicommands m_midi_commands { };   /* SysEx, derived midicommands      */

public:

    devicemodeenable () = default;
    devicemodeenable (const devicemodeenable & id) = default;
    devicemodeenable & operator = (const devicemodeenable & id) = default;
    devicemodeenable (devicemodeenable && id) = default;
    devicemodeenable & operator = (devicemodeenable && id) = default;
    ~devicemodeenable () = default;

    midicommands & midi_commands ()
    {
        return m_midi_commands;
    }

    const midicommands & midi_commands () const
    {
        return m_midi_commands;
    }

    void midi_commands (const midicommands & mc)
    {
        m_midi_commands = mc;
    }

};          // class devicemodeenable

/**
 *  ChannelNameSetAssign
 */

class channelnamesetassign
{

public:

    /**
     *  This is used by CustomDeviceMode.
     */

    using list = std::vector<channelnamesetassign>;

private:

    /**
     * The Channel number.
     */

    int m_channel;                      /* Channel; use a uint8_t?          */

    /**
     *  The name-set to be used.  INVESTIGATE!
     */

    std::string m_name_set;             /* NameSet                          */

public:

    channelnamesetassign () = default;
    channelnamesetassign (const channelnamesetassign &) = default;
    channelnamesetassign & operator =
    (
        const channelnamesetassign &
    ) = default;
    channelnamesetassign (channelnamesetassign &&) = default;
    channelnamesetassign & operator = (channelnamesetassign &&) = default;
    ~channelnamesetassign () = default;

    int channel () const
    {
        return m_channel;
    }

    void channel (int c)
    {
        m_channel = c;
    }

    const std::string & name_set ()
    {
        return m_name_set;
    }

    void name (const std::string & n)
    {
        m_name_set = n;
    }

};              // class channelnamesetassign

/**
 *  CustomDeviceMode.
 */

class customdevicemode
{

private:

    /**
     *  The "Name" of the this object.
     */

    std::string m_name { };

    /**
     *  Provides a MIDI command (likely to be SysEx) to enable a mode.
     */

    devicemodeenable m_device_mode_enable;

    /**
     *  A set of channels and name-sets.
     */

    channelnamesetassign::list m_name_set_assignments;

public:

    customdevicemode () = default;
    customdevicemode (const std::string & pname);
    customdevicemode (const customdevicemode & id) = default;
    customdevicemode & operator = (const customdevicemode & id) = default;
    customdevicemode (customdevicemode && id) = default;
    customdevicemode & operator = (customdevicemode && id) = default;
    ~customdevicemode () = default;

    const std::string & name ()
    {
        return m_name;
    }

    void name (const std::string & n)
    {
        m_name = n;
    }

    devicemodeenable & device_mode_enable ()
    {
        return m_device_mode_enable;
    }

    const devicemodeenable & device_mode_enable () const
    {
        return m_device_mode_enable;
    }

    void device_mode_enable (const devicemodeenable & dme)
    {
        m_device_mode_enable = dme;
    }

    channelnamesetassign::list & name_set_assignments ()
    {
        return m_name_set_assignments;
    }

    const channelnamesetassign::list & name_set_assignments () const
    {
        return m_name_set_assignments;
    }

    void name_set_assignments (const channelnamesetassign::list & cnsa)
    {
        m_name_set_assignments = cnsa;
    }

};          // class customdevicemode

/*--------------------------------------------------------------------------
 *  Free functions.
 *--------------------------------------------------------------------------*/

}           // namespace pack

}           // namespace midi

#endif      // MIDINAME_MIDI_PACK_CUSTOMDEVICEMODE_HPP

/*
 * customdevicemode.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
