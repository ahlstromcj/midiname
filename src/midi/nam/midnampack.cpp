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
 * \file          midnampack.cpp
 *
 *      Provides a way to fill in each of the classes in the midi::pack
 *      namespace.
 *
 * \library       midiname
 * \author        Chris Ahlstrom
 * \date          2026-03-20
 * \updates       2026-03-22
 * \license       See above.
 *
 */

#include <iostream>                     /* std::cout, std::cerr             */

#include "midiname.hpp"                 /* midiname_version() function      */
#include "midi/nam/midnampack.hpp"      /* midi::nam --> midi::pack         */

namespace midi
{

namespace nam
{

midnampack::midnampack (const std::string & midnamfile) :
    m_midnam_filename (midnamfile)
{
    // When should we call open()?
}

bool
midnampack::error (const std::string & msg)
{
    std::cout << "midnampack error: " << msg << std::endl;
    m_has_error = true;
    m_error_message = msg;
    return false;                       /* indicates a status of "error"    */
}

std::string
midnampack::patch_style_name () const
{
    std::string result { "Undetermined" };
    switch (patch_style())
    {
    case patchstyle::patch_name_list:

        result = "PatchNameList";
        break;

    case patchstyle::uses_patch_name_list:

        result = "UsesPatchNameList";
        break;

    case patchstyle::extending_device_list:

        result = "ExtendingDeviceList";
        break;

    default:

        break;
    }
    return result;
}

/**
 *  Processes a given file in the test-data directory.
 */

bool
midnampack::open (const std::string & midnamfile)
{
    xml66::XMLTree * xmldoc
    {
        new (std::nothrow) xml66::XMLTree(midnamfile)
    };
    bool result { bool(xmldoc) };
    if (result)
        result = xmldoc->is_valid();

    if (result)
    {
        xml66::SharedNodeListPtr found { xmldoc->find("//MIDINameDocument") };
        if (found->size() != 1)
            result = error("invalid MIDINameDocument");

        if (result)
        {
            found = xmldoc->find("//ExtendingDeviceName");
            if (found->size() != 0)
            {
                m_patch_style = patchstyle::extending_device_list;
            }
            else
            {
                found = xmldoc->find("//UsesPatchNameList");
                if (found->size() != 0)
                {
                    m_patch_style = patchstyle::uses_patch_name_list;
                }
                else
                {
                    found = xmldoc->find("//ChannelNameSet");
                    if (found->size() != 0)
                        m_patch_style = patchstyle::patch_name_list;
                }
            }
        }
        if (m_patch_style == patchstyle::undetermined)
            result = error("cannot determine the patch style");
    }
    if (result && is_verbose())
    {
        std::cout
            << "The patch style of " << patch_style_name()
            << "is " << midnam_filename() << "."
            << std::endl
            ;
    }
    return result;
}

/*
 *  These functions assume the unique XMLTree pointer is valid.
 */

/**
 *  Handles files that don't use the other methods.
 */

bool
midnampack::get_patch_name_list ()
{
#if defined THIS_CODE_IS_READY

    const xml66::XMLTree & doc { *xml_doc() };
    midi::nam::MasterDeviceNamesPtr masterDeviceNames
    {
        doc.master_device_names_by_model().find(model)->second
    };
    std::string modename
    {
        masterDeviceNames->custom_device_mode_names().front()
    };
    midi::nam::CustomDeviceModePtr mode
    {
        masterDeviceNames->custom_device_mode_by_name(modename)
    };
    midi::nam::ChannelNameSetPtr nameset
    {
        masterDeviceNames->channel_name_set_by_channel(modename, 0)
    };
    const midi::nam::PatchBanks & banks = nameset->patch_banks();
    if (banks.size() == 0)
        return error("No PatchBanks found in ChannelNameSet");

    midi::nam::PatchBankPtr bank { banks.front() };

    // We need to loop and collect

    const midi::nam::PatchNameList & plist { bank->patch_name_list() };
    if (plist.size() == 0)
        return error("No PatchNameList");

#endif
    return true;
}

/**
 *  Handles files whose patch-banks contain "UsesPatchNameList".
 *  An example is tests/data/Kurzweil_SP88x.midnam, which also
 *  has a "ChannelNameSet" holding a "PatchBank".
 */

bool
midnampack::get_uses_patch_name_list ()
{
    const xml66::XMLTree & doc { *xml_doc() };
    return true;
}

/**
 *  Handles files with "ExtendingDeviceNames". An example of this type
 *  is Lexicon_DR_Music_FX_Expansion.midnam.
 */

bool
midnampack::get_extending_device_list ()
{
    const xml66::XMLTree & doc { *xml_doc() };
    return true;
}

bool
midnampack::get_patch_banks ()
{
    bool result { false };
    switch (patch_style())
    {
    case patchstyle::patch_name_list:

        result = get_patch_name_list();
        break;

    case patchstyle::uses_patch_name_list:

        result = get_uses_patch_name_list();
        break;

    case patchstyle::extending_device_list:

        result = get_extending_device_list();
        break;

    default:

        break;
    }
    return result;
}

}           // namespace nam

}           // namespace midi

/*
 * midnampack.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
