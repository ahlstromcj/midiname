#if ! defined  MIDINAME_MIDI_NAM_MIDIPATCHMANAGER_HPP
#define MIDINAME_MIDI_NAM_MIDIPATCHMANAGER_HPP

/*
 *  This file is part of midiname.
 *
 *  midiname is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  midiname is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with midiname; if not, write to the Free Software Foundation, Inc., 59
 *  Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Copyright (C) 2008 Hans Baier <hansfbaier@googlemail.com>
 * Copyright (C) 2009-2014 David Robillard <d@drobilla.net>
 * Copyright (C) 2009-2014 Paul Davis <paul@linuxaudiosystems.com>
 * Copyright (C) 2015-2016 Robin Gareus <robin@gareus.org>
 * Copyright (C) 2015 Tim Mayberry <mojofunk@gmail.com>
 */

/**
 * \file          midipatchmanager.hpp
 *
 *    Provides the implementations for safe replacements for the various
 *    XML functions useful in handling MIDINAM files..
 *
 * \library       midiname library
 * \author        Chris Ahlstrom
 * \date          2026-02-20
 * \updates       2026-03-14
 * \version       $Revision$
 *
 */

#include "midi/nam/midnam_entities.hpp" /* class midi::nam::Patch, etc.     */
#include "util/searchpath.hpp"          /* class util::searchpath           */

/**
 *  In ardour/libs/pdb/search_path.cc/h, the Searchpath is
 *  our lib66::tokenization alias... a vector of strings.
 *
 *  We need additional functions to populate this vector.
 *
 *      -   Initialize the list of paths from a string where it
 *          contains one or more absolute paths to directories
 *          which are delimited by a path separation character.
 *          The path delimeter is a colon (":") on UNIX and a
 *          semi-colon (";") on windows. Each path contained in
 *          the search path may or may not resolve to an existing
 *          directory in the filesystem.
 *     -    Initialize the list of paths from a vector of paths.
 *          Each may or may not exist.
 *     -    Provide a function to construct the platform specific
 *          list string (with colon versus semicolon).
 *     -    Provide a function to add a set of search paths (or one
 *          search path) to the search paths.
 *     -    Removing entries from the search path.
 *     -    Seeing if the list contains(std::string).
 *
 *  These are now part of the cfg66 library class util::searchpath.
 *
 *  operator += (SearchPath)    and add_directory(vector<strings>)
 *  operator += (std::string)   and add_directory(string)
 *  operator + (SearchPath)
 *  operator + (std::string)
 *  operator -= (SearchPath)    and remove_directories(vector<string>)
 *  operator -= (std::string)   and remove_directory(string)
 *
 * Note:
 *
 *      This implementation doesn't support threads.
 */

/**
 *  The original implementation was a singleton class.
 */

#undef USE_SINGLETON_MIDIPATCHMANAGER

namespace midi
{

namespace nam
{

/*
 * Already defined in midnam_entities.hpp
 *
 *      using MIDINameDocumentPtr = std::shared_ptr<MIDINameDocument>;
 */

using MIDINameDocuments = std::map<std::string, MIDINameDocumentPtr>;

class MidiPatchManager
{

public:

    //  using MIDINameDocumentPtr = std::shared_ptr<MIDINameDocument>;
    //  using MIDINameDocuments = std::map<std::string, MIDINameDocumentPtr>;

    /*
     * TODO: we made this a "free" alias, moving it out of MIDINameDocument.
     * Might need to backtrack on that.
     *
     * using DeviceNamesList = MIDINameDocument::MasterDeviceNamesList>;
     *
     * Same for Models:
     *
     * using DeviceModels = MasterDeviceNames::Models;
     */

    using DeviceModels = Models;
    using DeviceNamesListEx = MasterDeviceNamesList;
    using DeviceNamesByMaker = std::map<std::string, DeviceNamesListEx>;

    /*
     * We migrate to a different entity, one not in pBD. In
     * ardour/libs/pdb/search_path.cc/h, the Searchpath is
     * our lib66::tokenization alias... a vector of strings.
     * Our util::searchpath class wraps that.
     *
     *      using SearchPath = PBD::Searchpath;
     */

    using SearchPath = util::searchpath;

private:

    util::searchpath m_search_paths { };
    MIDINameDocuments m_documents { };
    DeviceNamesListEx m_master_devices_by_model { };
    DeviceNamesByMaker m_devices_by_manufacturer { };
    DeviceModels m_all_models { };
    bool m_no_patch_changed_messages { false };

#if defined USE_SINGLETON_MIDIPATCHMANAGER

private:                    // this was a singleton class

    MidiPatchManager ();
    MidiPatchManager (const MidiPatchManager &);
    MidiPatchManager & operator = (const MidiPatchManager &);
    MidiPatchManager (MidiPatchManager &&);
    MidiPatchManager & operator = (MidiPatchManager &&);

#else

public:

    MidiPatchManager ();
    MidiPatchManager (const MidiPatchManager &) = default;
    MidiPatchManager & operator = (const MidiPatchManager &) = default;
    MidiPatchManager (MidiPatchManager &&) = default;
    MidiPatchManager & operator = (MidiPatchManager &&) = default;

#endif

public:

    ~MidiPatchManager ();

#if defined USE_SINGLETON_MIDIPATCHMANAGER

    static MidiPatchManager & instance ()
    {
        if (is_nullptr(m_manager))
        {
            m_manager = new MidiPatchManager();
        }
        return * m_manager;
    }

#endif

    util::searchpath & search_paths ()
    {
        return m_search_paths;
    }

    const util::searchpath & search_paths () const
    {
        return m_search_paths;
    }

    bool add_custom_midnam
    (
        const std::string & id,
        const std::string & midnam
    );
    bool update_custom_midnam
    (
        const std::string & id,
        const std::string & midnam
    );
    bool remove_custom_midnam (const std::string & id);
    bool is_custom_model (const std::string & model) const;

    void set_search_paths (const util::searchpath & search_paths)
    {
        m_search_paths = search_paths;
    }

    void add_search_paths (const util::searchpath & search_paths);
    void remove_search_paths (const util::searchpath & search_paths);

    void clear_search_paths ()
    {
        search_paths().paths().clear();
    }

    MIDINameDocumentPtr document_by_model
    (
        const std::string & modelname
    ) const;

    MasterDeviceNamesPtr master_device_by_model
    (
        const std::string & modelname
    )
    {
        return m_master_devices_by_model[modelname];
    }

    ChannelNameSetPtr find_channel_name_set
    (
        const std::string & model,
        const std::string & custom_device_mode,
        std::uint8_t channel
    );
    PatchPtr find_patch
    (
        const std::string & model,
        const std::string & custom_device_mode,
        std::uint8_t channel,
        PatchPrimaryKey patch_key
    );
    PatchPtr previous_patch
    (
        const std::string & model,
        const std::string & custom_device_mode,
        std::uint8_t channel,
        PatchPrimaryKey patch_key
    );
    PatchPtr next_patch
    (
        const std::string & model,
        const std::string & custom_device_mode,
        std::uint8_t channel,
        PatchPrimaryKey patch_key
    );

    // using CustomDeviceModeNames = std::list<std::string>;

    CustomDeviceModeNames custom_device_mode_names_by_model
    (
        const std::string & model_name
    );

    const DeviceModels & all_models () const
    {
        return m_all_models;
    }

    const DeviceNamesByMaker & devices_by_manufacturer () const
    {
        return m_devices_by_manufacturer;
    }


#if 0
    void load_midnams_in_thread ();
    void maybe_use (PBD::ScopedConnectionList & clist,
                    PBD::EventLoop::InvalidationRecord* ir,
                    const std::function<void()> & slot,
                    PBD::EventLoop* event_loop);
#endif

private:

    bool load_midi_name_document (const std::string & file_path);
    bool add_midi_name_document (MIDINameDocumentPtr);

    /*
     * Signal emitting parameter removed.
     */

    bool remove_midi_name_document (const std::string & file_path);
    void add_midnam_files_from_directory (const std::string & directory_path);
    void remove_midnam_files_from_directory (const std::string & directory_path);

private:

    void load_midnams ();

};          // class MidiPatchManager

}           // namespace nam

}           // namespace midi

#endif      // MIDINAME_MIDI_NAM_MIDIPATCHMANAGER_HPP

/*
 * midipatchmanager.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
