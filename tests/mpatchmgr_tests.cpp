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
 * \updates       2026-03-09
 * \license       See above.
 *
 *  Tests: (MPM is short for midi::nam::MidiPatchManager)
 *
 *      -   util::searchpath (Searchpath)
 *
 *          -   add_search_path(). Adds a util::searchpath to the MPM.
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
#include "midinam/midnam_entities.hpp"  /* midi::nam classes                */
#include "util/filefunctions.hpp"       /* util::get_env(), etc.            */
#include "util/ftswalker.hpp"           /* util::fts_get_file_path() etc.   */
#include "util/searchpath.hpp"          /* class util::searchpath           */

namespace   // anonymous
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

#if USE_THIS_CODE

class mpmgrtest
{

private:

    // util::searchpath m_search_paths;

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

	bool setup ()
    {
        return true;
    }

	bool teardown ()
    {
        return true;
    }

	bool protools_patchfile_test (const std::string & testpath);
	bool yamaha_PSRS900_patchfile_test (const std::string & testpath);
	bool load_all_midnams_test
    (
        const util::searchpath & srcpaths,
        bool verbose
    );

    void set_search_paths (util::searchpath & sp)
    {
        m_search_paths = sp;
    }

    util::searchpath & get_search_paths ()
    {
        return m_search_paths;
    }

    const util::searchpath & get_search_paths () const
    {
        return m_search_paths;
    }

public:     // used by main()

    bool set_test_search_paths
    (
        midi::nam::MidiPatchManager & mpm,
        const std::string & install_directory = ""
    );

};

#endif  // REMOVE_THIS_CODE

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
    midi::nam::MidiPatchManager & mpm,
    const std::string & installdirs
)
{
    util::searchpath spaths;            /* m_search_paths.paths().clear()   */
    mpm.clear_search_path(spaths);
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
        mpm.add_search_path(spaths);

    return result;
}

/**
 *  Processes a given file in the test-data directory.
 */

bool
mpmgrtest::protools_patchfile_test (const std::string & testpath)
{
    std::string testfilepath
    {
        util::fts_get_file_path(testpath, "Roland_SC_88_Pro.midnam")
    };
    (void) error(0, "protools_patchfile_test");
    if (testfilepath.empty())
        return error(1);

    xml66::XMLTree xmldoc(testfilepath);
    xml66::SharedNodeListPtr found { xmldoc.find("//MIDINameDocument") };
    if (found->size() != 1)
        return error(2);

    found = xmldoc.find("//ChannelNameSet");
    if (found->size() != 2)
        return error(3);

    midi::nam::MIDINameDocument doc(testfilepath);
    if (doc.all_models().size() != 1)
        return error(4);

    /*
     * In the file is:
     *
     * <Author>Mark of the Unicorn - converted from FreeMIDI, 4/28/04</Author>
     */

    std::string authortext { doc.author() };
    auto authorpos { authortext.find("Mark of the Unicorn") };
    if (authorpos > 0)
        return error(5);

    std::string model = *doc.all_models().begin();
    if (model != "SC-88 Pro")
        return error(6);

    midi::nam::MasterDeviceNamesPtr masterDeviceNames
    {
        doc.master_device_names_by_model().find(model)->second
    };
    if (masterDeviceNames->manufacturer() != "Roland")
        return error(7);

    std::string modename
    {
        masterDeviceNames->custom_device_mode_names().front()
    };
    if (modename != "Default")
        return error(8);

    midi::nam::CustomDeviceModePtr mode
    {
        masterDeviceNames->custom_device_mode_by_name(modename)
    };
    if (modename != mode->name())
        return error(9);

    std::string ns1 { "Name Set 1" };
    std::string ns2 { "Name Set 2" };
    for (uint8_t i = 0; i <= 15; ++i)
    {
        if (i != 9)
        {
            if (ns1 != mode->channel_name_set_name_by_channel(i))
                return error(10);
        }
        else
        {
            if (ns2 != mode->channel_name_set_name_by_channel(i))
                return error(11);
        }
    }

    midi::nam::ChannelNameSetPtr nameSet1
    {
        masterDeviceNames->channel_name_set_by_channel(modename, 0)
    };
    midi::nam::ChannelNameSetPtr nameSet2
    {
        masterDeviceNames->channel_name_set_by_channel(modename, 9)
    };
    if (ns1 != nameSet1->name())
        return error(12);

    if (ns2 != nameSet2->name())
        return error(13);

    const midi::nam::PatchBanks & banks1 = nameSet1->patch_banks();
    const midi::nam::PatchBanks & banks2 = nameSet2->patch_banks();
    if (banks1.size() != 16)
        return error(14);

    if (banks2.size() != 1)
        return error(15);

    midi::nam::PatchBankPtr bank { banks1.front() };
    if (bank->name() != "Piano")
        return error(16);

    const midi::nam::PatchNameList & plist1 { bank->patch_name_list() };
    if (plist1.size() != 110)
        return error(17);

    bank = banks2.front();
    if (bank->name() != "Drum sets")
        return error(18);

    const midi::nam::PatchNameList & plist2 { bank->patch_name_list() };
    if (plist2.size() != 49)
        return error(19);

    return true;
}

/**
 *
 */

bool
mpmgrtest::yamaha_PSRS900_patchfile_test (const std::string & testpath)
{
    std::string testfilepath
    {
        util::fts_get_file_path(testpath, "Yamaha_PSR_S900.midnam")
    };
    xml66::XMLTree xmldoc(testfilepath);
    xml66::SharedNodeListPtr pointers { xmldoc.find("//MIDINameDocument") };
    (void) error(0, "yamaha_PSRS900_patchfile_test");
    if (pointers->size() != 1)
        return error(1);

    pointers = xmldoc.find("//ChannelNameSet");
    if (pointers->size() != 3)
        return error(2);

    midi::nam::MIDINameDocument doc(testfilepath);
    if (doc.all_models().size() != 1)
        return error(3);

    if (doc.author().find("Hans Baier") != 0)
        return error(4);

    std::string model = *doc.all_models().begin();
    if (model != "PSR-S900")
        return error(5);

    midi::nam::MasterDeviceNamesPtr masterDeviceNames
    {
        doc.master_device_names_by_model().find(model)->second
    };
    if (masterDeviceNames->manufacturer() != "Yamaha")
        return error(6);

    const midi::nam::CustomDeviceModeNames & modes
    {
        masterDeviceNames->custom_device_mode_names()
    };
    if (masterDeviceNames->custom_device_mode_names().size() != 3)
        return error(16);       /* oops */

    std::string modename { modes.front() };
    if (modename != "Standard")
        return error(7);

    modename = (*(++modes.begin()));
    if (modename != "GM+XG")
        return error(8);

    modename = modes.back();
    if (modename != "GM2")
        return error(9);

    for (const auto & modename : modes)
    {
        midi::nam::CustomDeviceModePtr mode
        {
            masterDeviceNames->custom_device_mode_by_name(modename)
        };
        if (modename != mode->name())
            return error(10);

        std::string ns { mode->name() };
        if (ns != "Standard")
        {
            for (uint8_t i = 0; i <= 15; ++i)
            {
                if (ns != mode->channel_name_set_name_by_channel(i))
                    return error(11);

                midi::nam::ChannelNameSetPtr nameSet
                {
                    masterDeviceNames->channel_name_set_by_channel(ns, 1)
                };
                if (ns != nameSet->name())
                    return error(12);

                const midi::nam::PatchBanks & banks1
                {
                    nameSet->patch_banks()
                };
                if (banks1.size() <= 1)
                    return error(13);

                midi::nam::PatchBankPtr bank { banks1.front() };
                const midi::nam::PatchNameList & list
                {
                    bank->patch_name_list()
                };
                for (const auto & p : list)
                {
                    if (ns == "GM+XG")
                    {
                        uint8_t msb { uint8_t((p->bank_number() >> 7) & 0x7f) };
                        if (msb != 0 && msb != 64)
                            return error(14);
                    }

                    if (ns == "GM2")
                    {
                        if (p->bank_number() < (uint16_t(120) << 7))
                            return error(15);
                    }
                }
            }
        }
    }
    return true;
}

/**
 *  Needs some more work. Also need to test it against
 *  /usr/local/share/ardour9/patchfiles.
 *
 *  This test converts the wild-card "*.midnam" to a regular expression, then
 *  looks in the search-paths to find each file and work on it. The work
 *  for each file is:
 *
 *      -   Get the base-name (xyz.midnam) from the full path of the file
 *          to show it.
 *      -   Open a document := midi::nam::MIDINameDocument.
 *      -   Open an xmldoc := xml66::XMLTree.
 *      -   Verify that there is only one "//MIDINameDocument" node.
 *      -   Check for "//ExtendingDeviceNames", noting only if there are any.
 *      -   Verify that there is only one "//MasterDeviceNames" node.
 *      -   Find "//CustomDeviceMode[1]");
 *          -   Get the device mode name.
 *          -   Get the iterator to the first device (it's name).
 *          -   Get the name of the front custom device mode name from
 *              the first device (from the previous step).
 *          -   Get custom device mode pointer from the first device
 *              using the mode name found in the previous step.
 *          -   If the name() of the custom device mode pointer found
 *              in the previous step does not match the name found in
 *              the first sub-step, this is an error.
 *      -   Using the first device (found above), do some more work.
 *          -   Get a name-set pointer from the device using the mode name
 *              and index 0.
 *
 *              -- MORE TO DO --
 */

bool
mpmgrtest::load_all_midnams_test
(
    const util::searchpath & testsearchpaths,
    bool verbose
)
{
    lib66::tokenization tokens;
    std::string rgx { util::glob_to_regex("*.midnam") };
    util::fts_find_files_by_regex(tokens, testsearchpaths.paths(), rgx);
    std::cout
        << "Converted glob '*.midnam' to '" << rgx << "'."
        << std::endl
        ;
    (void) error(0, "load_all_midnams_test");
    if (tokens.empty())
        return error(1);

    std::cout
        << "Loading " << tokens.size() << " MIDINAM files from "
        << testsearchpaths.to_string()
        << std::endl
        ;

    for (auto fn : tokens)
    {
        std::string path, basename;
        bool ok
        {
            util::filename_split(fn, path, basename)
        };
        if (! ok)
            return error(2);

        if (verbose)
            std::cout << "Processing file " << basename << std::endl;

        midi::nam::MIDINameDocumentPtr document
        (
            new midi::nam::MIDINameDocument(fn)
        );
        xml66::XMLTree xmldoc(fn);
        xml66::SharedNodeListPtr nodes { xmldoc.find("//MIDINameDocument") };
        if (nodes->size() != 1)
            return error(3);

        nodes = xmldoc.find("//ExtendingDeviceNames");
        if (nodes->size() != 0)
        {
            if (verbose)
            {
                std::cout
                    << "   Found " << nodes->size()
                    << " ExtendingDeviceName(s), skipping them"
                    << std::endl
                    ;
            }
            continue;
        }

        nodes = xmldoc.find("//MasterDeviceNames");
        if (nodes->size() != 1)
            return error(4);

        // nodes = xmldoc.find("//PatchBank");    // int banks = nodes->size()

        nodes = xmldoc.find("//CustomDeviceMode[1]");

        std::string devicemodename
        {
            nodes->front()->property("Name")->value()
        };
        midi::nam::MasterDeviceNamesList::const_iterator device
        {
            document->master_device_names_by_model().begin()
        };
        std::string modename
        {
            device->second->custom_device_mode_names().front()
        };
        midi::nam::CustomDeviceModePtr mode
        {
            device->second->custom_device_mode_by_name(modename)
        };
        if (devicemodename != mode->name())
            return error(5);

        midi::nam::ChannelNameSetPtr nameset
        {
            device->second->channel_name_set_by_channel(modename, 0)
        };

        // MORE TO DO
    }
    return true;
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

}   // namespace anonymous

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
