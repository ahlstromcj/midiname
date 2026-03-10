/*
 *  This file is part of midiname.
 *
 *  midiname is free software; you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation; either version 2 of the License, or (at your option) any later
 *  version.
 *
 *  midiname is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with midiname; if not, write to the Free Software Foundation, Inc., 59 Temple
 *  Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * \file          mpatchmgr_tests.cpp
 *
 *      A test-file for the rudimentary XML/Midiname parser.
 *
 * \library       midiname
 * \author        Chris Ahlstrom
 * \date          2026-03-07
 * \updates       2026-03-10
 * \license       See above.
 *
 *  Tests: (MPM is short for midi::nam::MidiPatchManager)
 *
 *      -   util::searchpath (Searchpath)
 *
 *          $   add_search_paths(). Adds a util::searchpath to the MPM.
 *          -   remove_search_path(). Removes a search path from the MPM.
 *
 *      -   *.midnam handling
 *
 *          -   add_custom_midnam(). Creates a MIDINameDocument, reads a
 *              *.midnam file, and adds it to the patchmanager.
 *          -   load_midnams(). Call add_midnam_files_from_directory() for
 *              each directory in the search paths.
 *          -   add_midnam_files_from_directory(). Finds all *.midnam files in
 *              a directory and calls load_midi_name_document().
 *          -   load_midi_name_document(). Creates a new MIDINameDocument from
 *              a file-path, and calls add_midi_name_document().
 *          -   add_midi_name_document(). Complex. See the function banner.
 *          -   remove_midnam_files_from_directory(). Finds the matching *.midnam
 *              files and calls remove_midi_name_document() for each.
 *          -   remove_midi_name_document(). Complex. See the function banner.
 *          -   remove_custom_midnam() and update_custom_midnam().
 *
 *      - By-model selection:
 *
 *          -   is_custom_model(). Selects a document by model name and checks if
 *              it has "custom:" in its file-path.
 *          -   document_by_model(). Selects a document, by model name, from
 *              the collection of documents.
 *          -   custom_device_mode_names_by_model(). Collects the device mode
 *              names by model and returns a CustomDeviceModeNames object.
 *          -   find_channel_name_set(). Gets a master device by model name,
 *              and gets the set with the matching device mode and channel.
 *
 *      -   Patches:
 *
 *          -   find_patch(). Calls find_channel_name_set() and then looks up
 *              the patch key, returning a pointer to the patch.
 *          -   previous_patch(). Similar to find_patch(), but gets the patch
 *              previous to the given patch key.
 *          -   next_patch(). Similar to find_patch(), but gets the patch
 *              after to the given patch key.
 *
 * CURRENTLY IN PROGRESS
 * CURRENTLY IN PROGRESS
 * CURRENTLY IN PROGRESS
 */

#include <cstdlib>                      /* EXIT_SUCCESS, EXIT_FAILURE       */
#include <iostream>                     /* std::cout, std::cerr             */
#include <string>                       /* std::string                      */

#include "cli/parser.hpp"               /* cli::parser, etc.                */
#include "midiname.hpp"                 /* midiname_version() function      */
#include "midinam/midipatchmanager.hpp" /* midi::nam classes                */
#include "util/filefunctions.hpp"       /* util::get_env(), etc.            */
#include "util/ftswalker.hpp"           /* util::fts_get_file_path() etc.   */
#include "util/searchpath.hpp"          /* class util::searchpath           */

namespace           // anonymous
{

bool
error (int counter, const std::string & section = "")
{
    static std::string s_section { };
    if (counter == 0)
    {
        s_section = section;
        std::cout << "Processing test '" << s_section << "'" << std::endl;
        return true;
    }
    else
    {
        std::cerr << "   " << s_section << " error #" << counter << std::endl;
    }
    return false;
}

class mpmgrtest
{

private:

    midi::nam::MidiPatchManager m_patch_manager;

public:

    mpmgrtest () = default;
    mpmgrtest (const mpmgrtest &) = delete;
    mpmgrtest & operator = (const mpmgrtest &) = delete;
    mpmgrtest (mpmgrtest &&) = delete;
    mpmgrtest & operator = (mpmgrtest &&) = delete;
    ~mpmgrtest () = default;

    midi::nam::MidiPatchManager & patch_manager ()
    {
        return m_patch_manager;
    }

    const midi::nam::MidiPatchManager & patch_manager () const
    {
        return m_patch_manager;
    }

#if 0

	bool protools_patchfile_test (const std::string & testpath);
	bool yamaha_PSRS900_patchfile_test (const std::string & testpath);
	bool load_all_midnams_test
    (
        const util::searchpath & srcpaths,
        bool verbose
    );

#endif

    void set_search_paths (util::searchpath & sp)
    {
        patch_manager().set_search_paths(sp);
    }

    util::searchpath & get_search_paths ()
    {
        return patch_manager().search_paths();
    }

    const util::searchpath & get_search_paths () const
    {
        return patch_manager().search_paths();
    }

public:     // used by main()

    bool set_test_search_paths
    (
        const std::string & install_directory = ""
    );

};

/**
 *  Sets up the list of directories of test directories.
 *
 *  The MIDIPP_TEST_PATH environment variable can contain something like:
 *
 *      -   Linux:
 *          -   /usr/share/ardour9/patchfiles
 *          -   $XDG_DATA_HOME/share/clips
 *          -   $HOME/.local/share/sounds/clips/
 *      -   Windows:
 *          -   C:/ardour/share/ardour9/patchfiles ???
 *          -   C:\Program Files\Ardour9\share\ardour9\midi_maps
 *          -   %localappdata%\Ardour9\patchfiles
 *
 *  These hold a whole lot of *.midnam files.
 *
 * \param installdirs
 *      This value provides a PATH-style string with one or more
 *      directories in the path. The util::searchpath string constructor
 *      tokenizes this string and adds each directory to the searchpath
 *      object. If empty, then the directories will be gotten from
 *      the  MIDIPP_TEST_PATH environment variable, if it exists.
 *
 * \return
 *      Returns a copy of the constructed searchpath.
 */

bool
mpmgrtest::set_test_search_paths
(
    const std::string & installdirs
)
{
    util::searchpath & spaths { patch_manager().search_paths() };
    patch_manager().clear_search_paths();
    if (installdirs.empty())
    {
        std::string mtp { util::get_env("MIDIPP_TEST_PATH") };
        if (mtp.empty())
            spaths = util::searchpath("tests/data/");
        else
            spaths = util::searchpath(mtp);
    }
    else if (installdirs == "INSTALLED")
    {
        /*
         * This works only if Ardour 9 is installed.
         */

        std::string install_directory
        {
#if defined PLATFORM_WINDOWS
            "C:\\Program Files\\Ardour9"
#else
            "/usr/share/ardour9"
#endif
        };

        lib66::tokenization pathtoks;
        pathtoks.push_back(install_directory);
        pathtoks.push_back("share");
        pathtoks.push_back("ardour9");              // XXX lwrcase_dirname
        pathtoks.push_back("patchfiles");
        spaths = util::searchpath(pathtoks); // file_build_path(path_tok);
    }
    else
        spaths = util::searchpath(installdirs);

    bool result { spaths.paths().size() > 0 };
    if (result)
        patch_manager().add_search_paths(spaths);

    return result;
}

/*
 * Explanation text.
 */

const std::string help_intro
{
    "This test program illustrates and tests the C++ interface of the\n"
    "midiname library.  Options are as follows:\n\n"
};

/*--------------------------------------------------------------------------
 * Main
 *--------------------------------------------------------------------------*/

}           // namespace anonymous

/*
 * main() routine
 */

int
main (int argc, char * argv [])
{
    cli::parser clip;                   /* provides global/stock options    */
    bool adhoc_option_present { false };
    std::string testpath { clip.adhoc_option(argc, argv, "test-path") };
    if (! testpath.empty())
    {
        if (testpath == "?")
        {
            std::cout << "--test-path requires a filename." << std::endl;
        }
        else
        {
            std::cout << "--test-path '" << testpath << "'." << std::endl;
            adhoc_option_present = true;
        }
    }

    bool success { clip.parse(argc, argv) };
    bool nohelp { true };
    mpmgrtest mpmtest;
    std::cout << "Test of " << midiname_version() << ":" << std::endl;

    if (success)
    {
        if (clip.show_information_only())
        {
            nohelp = false;
std::cout
    << "The --verbose option greatly increases the output.\n"
    << "'--test-path directoryname' changes the test path from 'tests/data'.\n"
    << "For example, using '/usr/share/ardour9/patchfiles' if Ardour is\n"
    << "installed results in a ton of *.midnam files being processed, which.\n"
    << "takes awhile.\n"
    << std::endl
    ;
        }
    }
    if (nohelp)
    {
        if (success || adhoc_option_present)
        {
            if (! testpath.empty())                         /* --test-path  */
            {
                util::searchpath sp { testpath };
                mpmtest.set_search_paths(sp);
                success = true;
            }
            else
                success = mpmtest.set_test_search_paths();  /* defaults     */
        }

        if (success)
        {
            /*
             * We eventually need overloads of these first two that
             * use the already set up searchpath object. For now we
             * use the first path.
             */

#if 0
            std::string single_test_path
            {
                mpmtest.get_search_paths().paths()[0]
            };
            success = mpmtest.protools_patchfile_test(single_test_path);
            if (success)
            {
                success = mpmtest.yamaha_PSRS900_patchfile_test
                (
                    single_test_path
                );
            }
            if (success)
            {
                bool verbose { clip.verbose_request() };
                if (! verbose)
                    verbose = testpath.empty();

                success = mpmtest.load_all_midnams_test
                (
                    mpmtest.get_search_paths(), verbose
                );
            }
#endif
            if (success)
                std::cout << "mpatchmgr_tests has succeeded." << std::endl;
            else
                std::cerr << "mpatchmgr_tests has failed." << std::endl;
        }
    }
    return success ? EXIT_SUCCESS : EXIT_FAILURE ;
}

/*
 * mpatchmgr_tests.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
