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
 * \file          control.cpp
 *
 *  Provides data structures mirroring some of the classes in the
 *  midnam_entities module.
 *
 * \library       midiname library
 * \author        Chris Ahlstrom
 * \date          2026-03-28
 * \updates       2026-03-28
 * \version       $Revision$
 *
 *  This module contains the midi::pack classes related to controles.
 *  No XML code is involved; no pointers are involved.
 */

#include "midi/pack/control.hpp"        /* midi::pack::control class        */

namespace midi
{

namespace pack
{

/**
 *  control class.
 */

control::control
(
    const std::string & cname,
    type ctrltype,
    int ctrlnumber
) :
    m_ctrl_type     (ctrltype),
    m_ctrl_number   (number(ctrlnumber)),
    m_name          (cname)
{
    // no code
}

/*--------------------------------------------------------------------------
 *  Free functions.
 *--------------------------------------------------------------------------*/

std::string
control_type_name (control::type t)
{
    std::string result { "unknown" };
    switch (t)
    {
    case control::type::sevenbit:      result = "7bit";    break;
    case control::type::fourteenbit:   result = "14bit";   break;
    case control::type::rpn:           result = "RPN";     break;
    case control::type::nrpn:          result = "NRPN";    break;
    }
    return result;
}

control::type
string_to_control_type (const std::string & s)
{
    control::type result { control::type::sevenbit };
    if (s == "14bit")
        result = control::type::fourteenbit;
    else if (s == "RPN")
        result = control::type::rpn;
    else if (s == "NRPN")
        result = control::type::nrpn;

    return result;
}

}           // namespace pack

}           // namespace midi

/*
 * control.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
