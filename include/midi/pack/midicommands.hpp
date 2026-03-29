#if ! defined MIDINAME_MIDI_PACK_MIDICOMMANDS_HPP
#define MIDINAME_MIDI_PACK_MIDICOMMANDS_HPP

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
 * \file          midicommands.hpp
 *
 *  Provides data structures mirroring some of the classes in the
 *  midnam_entities module.
 *
 * \library       midiname library
 * \author        Chris Ahlstrom
 * \date          2026-03-23
 * \updates       2026-03-28
 * \version       $Revision$
 *
 *  This module defines the midi::pack::midicommands class. It's name is a
 *  bit misleading; it is used in a limited context.
 *
 *          MIDNAM Element          midi::pack class
 *
 *      -   MIDICommands            midicommands
 *      -   SysEx                   sysex
 */

#include "midi/pack/controlchange.hpp"  /* class midi::pack::controlchange  */

namespace midi
{

namespace pack
{

/**
 *  MidiCommands.
 *
 *  In a *.midnam file, there is one or two ControlChange elements:
 *
 *        <ControlChange Control="32" Value="0"/>
 *
 *  or
 *
 *        <ControlChange Control="0" Value="0"/>
 *        <ControlChange Control="32" Value="3"/>
 */

class midicommands
{

private:

    /**
     *  Matches the 'ControlChange' attribute. Generally there are one
     *  or two of these objects.
     */

    controlchange::list m_control_changes { };

public:

    midicommands () = default;

    midicommands
    (
        int control, int value,
        int control1 = (-1), int value1 = (-1)
    ) :
        m_control_changes   ()
    {
       controlchange cc { control, value };
       m_control_changes.push_back(cc);
       if (control1 >= 0)
       {
           controlchange cc2 { control1, value1 };
           m_control_changes.push_back(cc2);
       }
    }

    midicommands (const midicommands & id) = default;
    midicommands & operator = (const midicommands & id) = default;
    midicommands (midicommands && id) = default;
    midicommands & operator = (midicommands && id) = default;
    ~midicommands () = default;

    controlchange::list & control_changes ()
    {
        return m_control_changes;
    }

    const controlchange::list & control_changes () const
    {
        return m_control_changes;
    }

    void control_changes (const controlchange::list & cc)
    {
        m_control_changes = cc;
    }

};          // class midicommands

class sysex
{

private:

    /**
     *  Not quite sure of the meaning of this item yet.
     */

    int m_sysex_device_id_offset;

public:

    sysex (int offset = 0) : m_sysex_device_id_offset (offset)
    {
        // no code
    }

    sysex (const sysex & id) = default;
    sysex & operator = (const sysex & id) = default;
    sysex (sysex && id) = default;
    sysex & operator = (sysex && id) = default;
    ~sysex () = default;

    int sysex_device_id_offset () const
    {
        return m_sysex_device_id_offset;
    }

    void sysex_device_id_offset (int offset)
    {
        m_sysex_device_id_offset = offset;
    }

};          // class sysex

}           // namespace pack

}           // namespace midi

#endif      // MIDINAME_MIDI_PACK_MIDICOMMANDS_HPP

/*
 * midicommands.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
