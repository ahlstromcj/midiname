#if ! defined MIDINAME_MIDI_NAM_MIDINAMPACK_HPP
#define MIDINAME_MIDI_NAM_MIDINAMPACK_HPP

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
 * \file          midnampack.hpp
 *
 *      Provides a way to fill in each of the classes in the midi::pack
 *      namespace.
 *
 * \library       midiname
 * \author        Chris Ahlstrom
 * \date          2026-03-20
2* \updates       2026-03-23
 * \license       See above.
 *
 */

#include <memory>                       /* std::unique_ptr<>                */
#include <string>                       /* std::string                      */

#include "midiname.hpp"                 /* midiname_version() function      */
#include "midi/nam/midnam_entities.hpp" /* xml66 and midi::nam classes      */

namespace midi
{

namespace nam
{

class midnampack
{

public:

    /**
     *  Indicates the way patches are organized in the loaded *.midnam
     *  file. See the discussion in the extras/notes/get_patch_bank.text
     *  file.
     */

    enum class patchstyle
    {
        patch_name_list,
        uses_patch_name_list,
        extending_device_list,
        undetermined
    };

private:

    /**
     *  An abbreviation.
     */

    using tree = std::unique_ptr<xml66::XMLTree>;

private:

    /**
     *  Error notification items; a boolean and a string.
     */

    bool m_is_verbose { false };
    bool m_has_error { false };
    std::string m_error_message { };
    std::string m_midnam_filename { };

    /**
     *  Provides the data read from a *.midnam file.
     */

    tree m_xml_doc { };

    /**
     *  Provides the results of analyzing the *.midnam file for
     *  how PatchBanks are layed out.
     */

    patchstyle m_patch_style { patchstyle::undetermined };

public:

    midnampack (const std::string & midnamfile, bool isverbose = false);
    midnampack (const midnampack & id) = default;
    midnampack & operator = (const midnampack & id) = default;
    midnampack (midnampack && id) = default;
    midnampack & operator = (midnampack && id) = default;
    ~midnampack () = default;

    bool is_verbose () const
    {
        return m_is_verbose;
    }

    bool error (const std::string & msg);
    bool open ();

    bool has_error () const
    {
        return m_has_error;
    }

    const std::string & error_message ()
    {
        return m_error_message;
    }

    const std::string & midnam_filename () const
    {
        return m_midnam_filename;
    }

    std::string patch_style_name () const;

    patchstyle patch_style () const
    {
        return m_patch_style;
    }

    bool get_patch_banks ();

private:

    xml66::XMLTree & xml_doc ();

    const xml66::XMLTree & xml_doc () const
    {
        return std::as_const(xml_doc());
    }

    bool get_patch_name_list ();
    bool get_uses_patch_name_list ();
    bool get_extending_device_list ();

};          // class midnampack

}           // namespace nam

}           // namespace midi

#endif      // MIDINAME_MIDI_NAM_MIDINAMPACK_HPP

/*
 * midnampack.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
