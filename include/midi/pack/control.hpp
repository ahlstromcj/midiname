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
 * \updates       2026-03-16
 * \version       $Revision$
 *
 *  This module contains the midi::pack classes related to controles.
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
 *  control
 */

class control
{

public:

    using namelist = std::list<control>;
    using ctrlnumber = std::uint8_t;

    enum class ctrltype
    {
        sevenbit,
        fourteenbit,
        rpn,
        nrpn,
        max
    };

private:

    /**
     *  Provides broad characteristics of the control.
     */

    ctrltype m_ctrl_type { sevenbit };

    /**
     *  Provides the number of the control, the number that is sent and
     *  received.
     */

    ctrlnumber m_ctrl_number { 0 };      // for now

    /**
     *  The "Name" value of the "<Patch>" item.
     */

    std::string m_name { };

public:

    control () = default;
    control
    (
        const std::string & pname,
    );
    control (const control & id) = default;
    control & operator = (const control & id) = default;
    control (control && id) = default;
    control & operator = (control && id) = default;
    virtual ~control () = default;

    const ctrltype ctrl_type () const
    {
        return m_ctrl_type;
    }

    void ctrl_type (ctrltype ct)
    {
        m_ctrl_type = ct;
    }

    const ctrlnumber ctrl_number () const
    {
        return m_ctrl_number;
    }

    void ctrl_number (ctrlnumber cn)
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

extern std::string control_type_name (control::ctrltype t);
extern control::ctrltype (const std::string & s);

}           // namespace pack

}           // namespace midi

#endif      // MIDINAME_MIDI_PACK_CONTROL_HPP

/*
 * control.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
