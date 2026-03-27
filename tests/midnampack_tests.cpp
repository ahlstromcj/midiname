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
 * \file          midnampack_tests.cpp
 *
 *      A test-file for the midnampack class.
 *
 * \library       midiname
 * \author        Chris Ahlstrom
 * \date          2026-03-23
 * \updates       2026-03-25
 * \license       See above.
 *
 */

#include <cstdlib>                      /* EXIT_SUCCESS, EXIT_FAILURE       */
#include <iostream>                     /* std::cout, std::cerr             */
#include <string>                       /* std::string                      */

#include "cli/parser.hpp"               /* cli::parser, etc.                */
#include "midiname.hpp"                 /* midiname_version() function      */
#include "midi/nam/midnampack.hpp"      /* midi::nam::midnampack class      */

#include "midi/pack/midinamedocument.hpp"   /* */

namespace   // anonymous
{

/*
 * Explanation text.
 */

const std::string help_intro
{
    "This test program illustrates and tests the C++ interface of the\n"
    "midnamepack class.  Options are as follows:\n\n"
};

bool s_is_verbose { false };

bool
detect_patch_style
(
    const std::string & mnfilename,
    midi::nam::midnampack::patchstyle ps
)
{
    midi::nam::midnampack pack(mnfilename, s_is_verbose);
    bool result { pack.open() };
    if (result)
    {
        std::cout
            << mnfilename << " patch style " << pack.patch_style_name()
            << std::endl
            ;
        result = ps == pack.patch_style();
    }
    return result;
}

}           // namespace anonymous

/*--------------------------------------------------------------------------
 * Main
 *--------------------------------------------------------------------------*/

int
main (int argc, char * argv [])
{
    cli::parser clip;                   /* provides global/stock options    */
    bool success { clip.parse(argc, argv) };
    bool nohelp { true };
    std::cout
        << "Test of class midnampack in " << midiname_version() << ":"
        << std::endl
        ;

    if (success)
    {
        if (clip.show_information_only())
            nohelp = false;
    }
    if (nohelp && success)
    {
        s_is_verbose = clip.verbose_request();

        /*
         *  [ Roland_MT_32.midnam : patch_name_list ]
         *  [ Roland_SC_88_Pro.midnam : patch_name_list ]
         *  [ Yamaha_PSR_S900.midnam : patch_name_list ]
         *  [ Kurzweil_SP88x.midnam ]
         *  [ Lexicon_DR_Music_FX_Expansion.midnam ]
         */

        std::string filename { "tests/data/Roland_MT_32.midnam" };
        midi::nam::midnampack::patchstyle ps
        {
            midi::nam::midnampack::patchstyle::patch_name_list
        };
        success = detect_patch_style(filename, ps);
        if (success)
        {
            filename = "tests/data/Roland_SC_88_Pro.midnam";
            success = detect_patch_style(filename, ps);
        }
        if (success)
        {
            filename = "tests/data/Yamaha_PSR_S900.midnam";
            success = detect_patch_style(filename, ps);
        }
        if (success)
        {
            filename = "tests/data/Kurzweil_SP88x.midnam";
            ps = midi::nam::midnampack::patchstyle::uses_patch_name_list;
            success = detect_patch_style(filename, ps);
        }
        if (success)
        {
            filename = "tests/data/Lexicon_DR_Music_FX_Expansion.midnam";
            ps = midi::nam::midnampack::patchstyle::extending_device_list;
            success = detect_patch_style(filename, ps);
        }
        if (success)
            std::cout << "midnampack_tests has succeeded." << std::endl;
        else
            std::cerr << "midnampack_tests has failed." << std::endl;
    }
    return success ? EXIT_SUCCESS : EXIT_FAILURE ;
}

/*
 * midnampack_tests.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
