#if ! defined MIDINAME_MIDI_PACK_CONTROLCHANGE_HPP
#define MIDINAME_MIDI_PACK_CONTROLCHANGE_HPP

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
 * \file          controlchange.hpp
 *
 *  Provides data structures mirroring some of the classes in the
 *  midnam_entities module.
 *
 * \library       midiname library
 * \author        Chris Ahlstrom
 * \date          2026-03-23
 * \updates       2026-03-24
 * \version       $Revision$
 *
 *  This module defines the midi::pack::controlchange class. It is
 *  used in the "MIDICommands" and "PatchMIDICommands" elements.
 *  Usually, there is either one ControlChange in a MIDICommands,
 *  or two ControlChanges plus a ProgramChange in a PatchMIDICommands.
 */

namespace midi
{

namespace pack
{

/**
 *  ControlChange. A small object used in conjunction with
 *  PatchMIDICommands. It provides a value needed to change to a
 *  different patch.
 *
 *  If present, a common case is inside a PatchMIDICommands, with two of
 *  these objects, one with Control = 0, and the other with Control =
 *  32.  RPN, NRPN?
 *
 *  Another case in PatchMIDICommands is just one object with Control =
 *  0.  In them we see the patch number ending with a "+" or a "#".
 *
 *  This name is used as an attribute in a Patch or PatchMIDICommands
 *  entity. It is defined in MIDIEvents.dtd.
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

class controlchange
{

public:

    using list = std::vector<controlchange>;

private:

    /**
     *  Attribute: "Control"
     *
     *  The control number. It's stored here as an integer, but it is an
     *  8-bit quantity (std::uint8_t).
     */

    int m_ctrl_number { 0 };

    /**
     *  Attribute: "Value"
     *
     *  The value of the control. It's stored here as an integer, but it is
     *  an 8-bit quantity (std::uint8_t).
     */

    int m_ctrl_value { 0 };

public:

    controlchange () = default;

    controlchange (int controlno, int value) :
        m_ctrl_number   (controlno),
        m_ctrl_value    (value)
    {
        // no code
    }

    controlchange (const controlchange & id) = default;
    controlchange & operator = (const controlchange & id) = default;
    controlchange (controlchange && id) = default;
    controlchange & operator = (controlchange && id) = default;
    ~controlchange () = default;

    int ctrl_number () const
    {
        return m_ctrl_number;
    }

    int ctrlk_value () const
    {
        return m_ctrl_value;
    }

};          // class controlchange

}           // namespace pack

}           // namespace midi

#endif      // MIDINAME_MIDI_PACK_CONTROLCHANGE_HPP

/*
 * controlchange.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
