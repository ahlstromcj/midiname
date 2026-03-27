#if ! defined MIDINAME_MIDI_PACK_EXTENDINGDEVICENAMES_HPP
#define MIDINAME_MIDI_PACK_EXTENDINGDEVICENAMES_HPP

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
 * \file          extendingdevicenames.hpp
 *
 *  Provides data structures mirroring some of the classes in the
 *  midnam_entities module.
 *
 * \library       midiname library
 * \author        Chris Ahlstrom
 * \date          2026-03-25
 * \updates       2026-03-27
 * \version       $Revision$
 *
 *  This module contains the midi::pack classes related to
 *  extendingdevicenameses.  No XML code is involved; no pointers are
 *  involved.
 */

#include <string>                       /* std::string<>                    */

namespace midi
{

namespace pack
{

/**
 *  extendingdevicenames
 */

class extendingdevicenames
{


private:

    /**
     *  The "Name" of the this object.
     */

    std::string m_name { };

public:

    extendingdevicenames () = default;
    extendingdevicenames (const std::string & pname);
    extendingdevicenames (const extendingdevicenames & id) = default;
    extendingdevicenames & operator =
    (
        const extendingdevicenames & id
    ) = default;
    extendingdevicenames (extendingdevicenames && id) = default;
    extendingdevicenames & operator = (extendingdevicenames && id) = default;
    ~extendingdevicenames () = default;

    const std::string & name ()
    {
        return m_name;
    }

    void name (const std::string & n)
    {
        m_name = n;
    }

};          // class extendingdevicenames

/*--------------------------------------------------------------------------
 *  Free functions.
 *--------------------------------------------------------------------------*/

}           // namespace pack

}           // namespace midi

#endif      // MIDINAME_MIDI_PACK_EXTENDINGDEVICENAMES_HPP

/*
 * extendingdevicenames.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
