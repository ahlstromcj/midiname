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
 * \updates       2026-03-27
 * \version       $Revision$
 *
 *  This module contains the midi::pack classes related to valuees.
 *  No XML code is involved; no pointers are involved.
 *
 *      -   values
 *      -   value
 *      -   values::list (ValueNameList)
 *      -   usesvaluenamelist (string)
 */

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

    using list = std::list<value>;

private:

    /**
     *  The "Name" value of the "<Value>" item.
     */

    std::string m_name { };

    /**
     *  The "Number" value of the Value.
     */

    int m_value_number;

public:

    value () = default;

    value (const std::string & pname, int pnumber) :
        m_name      (pname),
        m_number    (pnumber)
    {
        // no code
    }

    value (const value & id) = default;
    value & operator = (const value & id) = default;
    value (value && id) = default;
    value & operator = (value && id) = default;
    ~value () = default;

    const std::string & name () const
    {
        return m_name;
    }

    void name (const std::string & name)
    {
        m_name = name;
    }

    int value_number () const
    {
        return m_value_number;
    }

    void value_number (int v)
    {
        m_value_number = v;
    }

};          // class value

/**
 *  Values.
 */

class values
{

private:

    /**
     *  Min.
     */

    int m_minimum { 0 };

    /**
     *  Max.
     */

    int m_maximum { 127 };

    /**
     *  Default.
     */

    int m_default { 0 };

    /**
     *  Units.
     */

    std::string m_units { };

    /**
     *  Mapping.
     */

    std::string m_mapping { };

public:

    values () = default;
    values (const values & id) = default;
    values & operator = (const values & id) = default;
    values (values && id) = default;
    values & operator = (values && id) = default;
    ~values () = default;

    // TODO getters and setters

};          // class values

}           // namespace pack

}           // namespace midi

#endif      // MIDINAME_MIDI_PACK_VALUE_HPP

/*
 * value.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
