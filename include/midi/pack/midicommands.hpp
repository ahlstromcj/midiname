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
 * \date          2026-03-17
 * \updates       2026-03-17
 * \version       $Revision$
 *
 *  This module contains the midi::pack classes related to midicommands.
 *  No XML code is involved; no pointers are involved.
 */

#include <cstdint>                      /* std::uint16_t, uint8_t           */
#include <list>                         /* std::list<>                      */
#include <map>                          /* std::map<>                       */
#include <string>                       /* std::string<>                    */

namespace midi
{

namespace pack
{

/**
 * This element is not found in any of Ardour's patchfiles.
 */

class mididelay
{

private:

    int m_milliseconds;

public:

    mididelay () = default;

    mididelay (int ms) : m_milliseconds (ms)
    {
        // no code
    }

    mididelay (const mididelay & id) = default;
    mididelay & operator = (const mididelay & id) = default;
    mididelay (mididelay && id) = default;
    mididelay & operator = (mididelay && id) = default;
    ~mididelay () = default;

    int milliseconds () const
    {
        return m_milliseconds;
    }

};          // class mididelay

/**
 *  This class is just a placeholder for MIDIChannelMessage, which
 *  is defined in MIDIEvents10.dtd. It covers more than we need to
 *  deal with at this time.
 *
 *  MIDIChannelVoiceMessage:
 *
 *      NoteOn | NoteOff | PolyKeyPressure | ControlChange | ProgramChange |
 *      ChannelKeyPressure | PitchBendChange
 *
 *  MIDIChannelModeMessage:
 *
 *      AllSoundOff | ResetAllControllers | LocalControl | AllNotesOff |
 *      OmniOff | OmniOn | MonoMode | PolyMode
 *
 *  MIDIChannelMessage:
 *
 *      %MIDIChannelVoiceMessage; | %MIDIChannelModeMessage
 *
 *  MIDISystemMessage:
 *
 *      SysEx | MTCQuarterFrame | SongPositionPointer | SongSelect |
 *      TuneRequest | TimingClock | Start | Continue | Stop |
 *      ActiveSensing | SystemReset
 *
 *  MIDIMessage:
 *
 *      %MIDIChannelMessage | %MIDISystemMessage
 */

class midichannelmessage
{

public:

    midichannelmessage () = default;
    midichannelmessage (const midichannelmessage & id) = default;
    midichannelmessage & operator = (const midichannelmessage & id) = default;
    midichannelmessage (midichannelmessage && id) = default;
    midichannelmessage & operator = (midichannelmessage && id) = default;
    ~midichannelmessage () = default;

};          // class midichannelmessage

/**
 *  A simple ID code.
 */

class sysexdeviceid
{

private:

    int m_offset;

public:

    sysexdeviceid () = default;

    sysexdeviceid (int offset) : m_offset (offset)
    {
        // no code
    }

    sysexdeviceid (const sysexdeviceid & id) = default;
    sysexdeviceid & operator = (const sysexdeviceid & id) = default;
    sysexdeviceid (sysexdeviceid && id) = default;
    sysexdeviceid & operator = (sysexdeviceid && id) = default;
    ~sysexdeviceid () = default;

    int offset () const
    {
        return m_offset;
    }

};          // class sysexdeviceid

/**
 *  Format:
 *
 *      F0 manid devid modelid direction address   data checksum terminator
 *      F0 0x41  0x10  0x42    0x12      0x40007F  0x00 0x41     0xF7
 *
 *  <SysEx>
 *      F0 41 <SysExDeviceID Offset="00"/> 42 12 40 00 7F 00 41 F7
 *  </SysEx>
 *
 *  For now, we just get the bytes.
 */

class sysex
{

private:

    sysexdeviceid m_device_id;

    unsigned char * m_message_bytes;

    std::size_t m_message_size;

public:

    sysex () = default;
    sysex (const sysex & id) = delete;
    sysex & operator = (const sysex & id) = delete;
    sysex (sysex && id) = default;
    sysex & operator = (sysex && id) = default;
    ~sysex () = default;    // FIXME

};          // class sysex

/**
 *  midicommands
 */

class midicommands
{

public:

    // using namelist = std::list<midicommands>;
    // using bank_number = std::uint16_t;
    // using number = std::uint8_t;

private:

    /**
     *  The "Name" midicommands of the "<Patch>" item.
     */

    std::string m_name { };

    /**
     *  Need to investigate this one.
     */

public:

    midicommands () = default;
    midicommands
    (
        const std::string & pname
    );
    midicommands (const midicommands & id) = default;
    midicommands & operator = (const midicommands & id) = default;
    midicommands (midicommands && id) = default;
    midicommands & operator = (midicommands && id) = default;
    ~midicommands () = default;

    const std::string & name () const
    {
        return m_name;
    }

    void set_name (const std::string & name)
    {
        m_name = name;
    }

};          // class midicommands

}           // namespace pack

}           // namespace midi

#endif      // MIDINAME_MIDI_PACK_MIDICOMMANDS_HPP

/*
 * midicommands.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
