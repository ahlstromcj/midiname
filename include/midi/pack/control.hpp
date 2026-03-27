#if ! defined MIDINAME_MIDI_PACK_CONTROL_HPP
#define MIDINAME_MIDI_PACK_CONTROL_HPP

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
 * \file          control.hpp
 *
 *  Provides data structures mirroring some of the classes in the
 *  midnam_entities module.
 *
 * \library       midiname library
 * \author        Chris Ahlstrom
 * \date          2026-03-16
 * \updates       2026-03-26
 * \version       $Revision$
 *
 *  This module contains the midi::pack classes related to controles.
 *  No XML code is involved; no pointers are involved.
 */

#include <cstdint>                      /* std::uint16_t, uint8_t           */
#include <string>                       /* std::string<>                    */
#include <vector>                       /* std::vector<>                    */

namespace midi
{

namespace pack
{

/**
 *  control
 */

class control
{

public:

    /**
     *  A handy alias for one-byte MIDI values. We might need more types
     *  as per the enum class type below.
     */

    using number = std::uint8_t;

    /**
     *  Provides a type that can be used by control-containing elements.
     */

    using list = std::vector<control>;

    /**
     *  The type of the control.
     */

    enum class type
    {
        sevenbit,                       /* "7bit": the default value        */
        fourteenbit,                    /* "14bit"                          */
        rpn,                            /* "RPN"                            */
        nrpn                            /* "NRPN"                           */
    };

private:

    /**
     *  Provides broad characteristics of the control.
     */

    type m_ctrl_type { type::sevenbit };

    /**
     *  Provides the number of the control, the number that is sent and
     *  received.
     */

    number m_ctrl_number { 0 };         /* for now */

    /**
     *  The "Name" value of the "<Patch>" item.
     */

    std::string m_name { };

public:

    control () = default;
    control (const std::string & pname);
    control (const control & id) = default;
    control & operator = (const control & id) = default;
    control (control && id) = default;
    control & operator = (control && id) = default;
    virtual ~control () = default;

    const type ctrl_type () const
    {
        return m_ctrl_type;
    }

    void ctrl_type (type ct)
    {
        m_ctrl_type = ct;
    }

    const ctrl_number ctrl_number () const
    {
        return m_ctrl_number;
    }

    void ctrl_number (number cn)
    {
        m_ctrl_number = cn;
    }

    const std::string & name () const
    {
        return m_name;
    }

    void set_name (const std::string & name)
    {
        m_name = name;
    }

};          // class control

/*--------------------------------------------------------------------------
 *  Free functions.
 *--------------------------------------------------------------------------*/

extern std::string control_type_name (control::type t);
extern control::type (const std::string & s);

}           // namespace pack

}           // namespace midi

#endif      // MIDINAME_MIDI_PACK_CONTROL_HPP

/*
 * control.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
