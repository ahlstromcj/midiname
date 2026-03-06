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
 * \file          midiname.cpp
 *
 *  This module defines an informational function.
 *
 * \library       midiname
 * \author        Chris Ahlstrom
 * \date          2026-02-21
 * \updates       2026-02-21
 * \license       GNU GPLv2 or above
 *
 */

#include "midiname.hpp"                 /* no-namespace function library    */

/*
 * Version information string.
 */

const std::string &
midiname_version () noexcept
{
    static std::string s_info
    {
        MIDINAME_NAME "-" MIDINAME_VERSION " " __DATE__
    };
    return s_info;
}

/*
 * midiname.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
