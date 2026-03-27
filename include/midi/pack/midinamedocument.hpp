#if ! defined MIDINAME_MIDI_PACK_MIDINAMEDOCUMENT_HPP
#define MIDINAME_MIDI_PACK_MIDINAMEDOCUMENT_HPP

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
 * \file          midinamedocument.hpp
 *
 *  Provides data structures mirroring some of the classes in the
 *  midnam_entities module.
 *
 * \library       midiname library
 * \author        Chris Ahlstrom
 * \date          2026-03-24
 * \updates       2026-03-27
 * \version       $Revision$
 *
 *  This module contains the midi::pack classes related to midinamedocumentes.
 *  No XML code is involved; no pointers are involved.
 */

#include <string>                       /* std::string<>                    */

#include "midi/pack/customdevicemode.hpp"       /* midi::pack::custom...    */
#include "midi/pack/masterdevicenames.hpp"      /* midi::pack::master...    */
#include "midi/pack/extendingdevicenames.hpp"   /* midi::pack::extending... */

namespace midi
{

namespace pack
{

/**
 *  MidiNameDocument. The whole of the contents of a *.midname file,
 *  excepting the XML header.
 */

class midinamedocument
{

public:


private:

    /**
     *  The "Name" of the document, its file-name.
     */

    std::string m_name { };

    /**
     *  Represents the <Author> element.
     */

    std::string m_author { };

    /**
     *  The optional MasterDeviceNames.
     */

    masterdevicenames m_master_device_names;

    /**
     *  The optional ExtendingDeviceNames.
     */

    extendingdevicenames m_extending_device_names;

    /**
     *  The optional StandardDeviceModel. No Ardourd patch-file
     *  includes this element. We will belay making this class
     *  until we find out how it can be used.
     *
     *      standarddevicemode m_standard_device_model;
     */

    /**
     *  The optional CustomDeviceMode.
     */

    customdevicemode m_custom_device_mode;

public:

    midinamedocument () = default;
    midinamedocument (const std::string & pname);
    midinamedocument (const midinamedocument & id) = default;
    midinamedocument & operator = (const midinamedocument & id) = default;
    midinamedocument (midinamedocument && id) = default;
    midinamedocument & operator = (midinamedocument && id) = default;
    ~midinamedocument () = default;

    const std::string & name () const
    {
        return m_name;
    }

    void name (const std::string & n)
    {
        m_name = n;
    }

    masterdevicenames & master_device_names ()
    {
        return m_master_device_names;
    }

    const masterdevicenames & master_device_names () const
    {
        return m_master_device_names;
    }

    void master_device_names (const masterdevicenames & mdn)
    {
        m_master_device_names = mdn;
    }

    extendingdevicenames & extending_device_names ()
    {
        return m_extending_device_names;
    }

    const extendingdevicenames & extending_device_names () const
    {
        return m_extending_device_names;
    }

    void extending_device_names (const extendingdevicenames & edn)
    {
        m_extending_device_names = edn;
    }

    customdevicemode & custom_device_mode ()
    {
        return m_custom_device_mode;
    }

    const customdevicemode & custom_device_mode () const
    {
        return m_custom_device_mode;
    }

    void custom_device_mode (const customdevicemode & cdm)
    {
        m_custom_device_mode = cdm;
    }

};          // class midinamedocument

/*--------------------------------------------------------------------------
 *  Free functions.
 *--------------------------------------------------------------------------*/

}           // namespace pack

}           // namespace midi

#endif      // MIDINAME_MIDI_PACK_MIDINAMEDOCUMENT_HPP

/*
 * midinamedocument.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
