#if ! defined MIDINAME_MIDI_PACK_VALUE_HPP
#define MIDINAME_MIDI_PACK_VALUE_HPP

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
 * \file          value.hpp
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
 *  This module contains the midi::pack classes related to valuees.
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
 *  value
 */

class value
{

public:

    using namelist = std::list<value>;
    using bank_number = std::uint16_t;
    using number = std::uint8_t;

private:

    /**
     *  The "Name" value of the "<Patch>" item.
     */

    std::string m_name { };

    /**
     *  Need to investigate this one.
     */

public:

    value () = default;
    value
    (
        const std::string & pname,
    );
    value (const value & id) = default;
    value & operator = (const value & id) = default;
    value (value && id) = default;
    value & operator = (value && id) = default;
    virtual ~value () = default;

    const std::string & name () const
    {
        return m_name;
    }

    void set_name (const std::string & name)
    {
        m_name = name;
    }

};          // class value

}           // namespace pack

}           // namespace midi

#endif      // MIDINAME_MIDI_PACK_VALUE_HPP

/*
 * value.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
