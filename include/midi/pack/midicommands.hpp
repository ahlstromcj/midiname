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
 * \updates       2026-03-24
 * \version       $Revision$
 *
 *  This module defines the midi::pack::midicommands class. It's name is a
 *  bit misleading; it is used in a limited context.
 */

#include <vector>                       /* std::vector<>                    */

#include "midi/pack/controlchange.hpp"  /* class midi::pack::controlchange  */

namespace midi
{

namespace pack
{

/**
 *  midicommands.
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
     *  or two fo these objects.
     */

    std::vector<controlchange> m_control_changes { };

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

    // more to do

};          // class midicommands

}           // namespace pack

}           // namespace midi

#endif      // MIDINAME_MIDI_PACK_MIDICOMMANDS_HPP

/*
 * midicommands.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
