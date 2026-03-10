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
 * Copyright (C) 2009-2016 Paul Davis <paul@linuxaudiosystems.com>
 * Copyright (C) 2012-2015 Tim Mayberry <mojofunk@gmail.com>
 * Copyright (C) 2014-2016 Robin Gareus <robin@gareus.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/**
 * \file          midipatchmanager.cpp
 *
 *    Provides the implementations for safe replacements for the various
 *    XML functions useful in handling MIDINAM files.
 *
 * \library       midiname library
 * \author        Chris Ahlstrom
 * \date          2026-02-20
 * \updates       2026-03-10
 * \version       $Revision$
 *
 */

#include "midinam/midipatchmanager.hpp" /* midi::nam::MidiPatchManager      */
#include "util/filefunctions.hpp"       /* util::file_exists()              */
#include "util/ftswalker.hpp"           /* util::fts_find_files_by_regex()  */

namespace midi
{

namespace nam
{

/**
 *  Provides the singleton MidiPatchManager. We don't care about that now.
 *
 *      MidiPatchManager * MidiPatchManager::m_manager { nullptr };
 */

/**
 *  Constructor.
 */

MidiPatchManager::MidiPatchManager ()
{
    // this is from ardour's search_paths.cc/h.
    //
    // add_search_path(midi_patch_search_path());
}

/**
 *  Destructor.
 */

MidiPatchManager::~MidiPatchManager ()
{
#if defined USE_SINGLETON_MIDIPATCHMANAGER
    m_manager = nullptr;            // why bother? shouldn't we delete it?
#endif
}

/**
 *  Adds the files in a util::searchpath to the patch-manager's util::searchpath
 */

void
MidiPatchManager::add_search_paths (const util::searchpath & srch_paths)
{
    for (const auto & f : srch_paths.paths())
    {
        if (search_paths().contains(f))
            continue;               // already processed files from this path

        if (util::file_exists(f))
            continue;

        if (util::file_is_directory(f))
            continue;

        search_paths().add_directory(f);
    }
}

/**
 *  Creates a MIDINameDocument, reads a *.midnam file into an XMLTree,
 *  and sets the MPM's state to the tree. It sets the file-path of
 *  this document to "custom:ID".
 */

bool
MidiPatchManager::add_custom_midnam
(
    const std::string & id,
    const std::string & midnam
)
{
    MIDINameDocumentPtr document
    {
        MIDINameDocumentPtr(new MIDINameDocument())
    };
    xml66::XMLTree mxml;
    if (mxml.read_buffer(CSTR(midnam), true))
    {
        if (document->set_state(mxml, *mxml.root()) == 0)
        {
            document->set_file_path("custom:" + id);
            add_midi_name_document(document);
            return true;
        }
    }
    return false;
}

bool
MidiPatchManager::remove_custom_midnam (const std::string & id)
{
    return remove_midi_name_document("custom:" + id);
}

bool
MidiPatchManager::update_custom_midnam
(
    const std::string & id,
    const std::string & midnam
)
{
    remove_midi_name_document("custom:" + id);  /* no more signalling here  */
    return add_custom_midnam(id, midnam);
}

bool
MidiPatchManager::is_custom_model (const std::string & model) const
{
    MIDINameDocumentPtr midnam { document_by_model(model) };
    return not_nullptr(midnam) &&
        midnam->file_path().substr(0, 7) == "custom:";
}

void
MidiPatchManager::add_midnam_files_from_directory
(
    const std::string & directory_path
)
{
    lib66::tokenization collected;
    lib66::tokenization paths { directory_path };                   /* ugh  */
    std::string rgx { util::glob_to_regex("*.midnam") };
    (void) util::fts_find_files_by_regex(collected, paths, rgx);

    // P_() is a variation on dgettext() and figures out plurals.
    // P_("Loading %1 MIDI patch from %2",
    //     "Loading %1 MIDI patches from %2", collected.size()),
    //     collected.size(), directory_path) << endmsg;

    std::string msg
    {
        util::string_format
        (
            "Loading %1 MIDI patch(es) from %2",
            collected.size(), V(directory_path)
        )
    };
    std::cout << msg << std::endl;
    for (auto i : collected)
    {
        // if (stop_thread)
        //     break;

        load_midi_name_document(i);
    }
}

void
MidiPatchManager::remove_search_paths (const util::searchpath & srch_paths)
{
    for (auto i : srch_paths.paths())
    {
        if (! search_paths().contains(i))
            continue;

        remove_midnam_files_from_directory(i);
        search_paths().remove_directory (i);
    }
}

void
MidiPatchManager::remove_midnam_files_from_directory
(
    const std::string & directory_path
)
{
    lib66::tokenization collected;
    lib66::tokenization paths { directory_path };                   /* ugh  */
    std::string rgx { util::glob_to_regex("*.midnam") };
    (void) util::fts_find_files_by_regex(collected, paths, rgx);

    std::string msg
    {
        util::string_format
        (
            "Unloading %zu MIDI patch(es) from %s",
            collected.size(), V(directory_path)
        )
    };
    std::cout << msg << std::endl;
    for (auto i : collected)
        remove_midi_name_document(i);
}

bool
MidiPatchManager::load_midi_name_document (const std::string & file_path)
{
    MIDINameDocumentPtr document;
    try
    {
        document = MIDINameDocumentPtr(new MIDINameDocument(file_path));
    }
    catch (...)                     // xml66::XMLException ?
    {
        std::cerr
            << "Error parsing MIDI patch file " << file_path
            << std::endl
            ;
        return false;
    }
    return add_midi_name_document(document);
}

/* MidiPatchManager:: */

MIDINameDocumentPtr
MidiPatchManager::document_by_model (const std::string & modelname) const
{
    MIDINameDocuments::const_iterator i { m_documents.find(modelname) };
    if (i != m_documents.end ())
        return i->second;

    return MIDINameDocumentPtr();
}


ChannelNameSetPtr
MidiPatchManager::find_channel_name_set
(
    const std::string & model,
    const std::string & customdevicemode,
    std::uint8_t channel
)
{
    MasterDeviceNamesPtr masterdevice { master_device_by_model(model) };
    if (not_nullptr(masterdevice) && ! customdevicemode.empty())
    {
        return masterdevice->channel_name_set_by_channel
        (
            customdevicemode, channel
        );
    }
    else
        return ChannelNameSetPtr();
}

/* MidiPatchManager:: */

PatchPtr
MidiPatchManager::find_patch
(
    const std::string & model,
    const std::string & custom_device_mode,
    std::uint8_t channel,
    PatchPrimaryKey patch_key
)
{
    ChannelNameSetPtr channel_name_set
    {
        find_channel_name_set(model, custom_device_mode, channel)
    };
    if (not_nullptr(channel_name_set))
        return channel_name_set->find_patch(patch_key);
    else
        return PatchPtr();
}

/* MidiPatchManager:: */

PatchPtr
MidiPatchManager::previous_patch
(
    const std::string & model,
    const std::string & custom_device_mode,
    std::uint8_t channel,
    PatchPrimaryKey patch_key
)
{
    ChannelNameSetPtr channel_name_set
    {
        find_channel_name_set(model, custom_device_mode, channel)
    };
    if (not_nullptr(channel_name_set))
        return channel_name_set->previous_patch(patch_key);
    else
        return PatchPtr();
}

/* MidiPatchManager:: */

PatchPtr
MidiPatchManager::next_patch
(
    const std::string & model,
    const std::string & custom_device_mode,
    std::uint8_t channel,
    PatchPrimaryKey patch_key
)
{
    ChannelNameSetPtr channel_name_set
    {
        find_channel_name_set(model, custom_device_mode, channel)
    };
    if (not_nullptr(channel_name_set))
        return channel_name_set->next_patch(patch_key);
    else
        return PatchPtr();
}

/* MidiPatchManager:: */ // CustomDeviceModeNames = std::list<std::string>;

CustomDeviceModeNames
MidiPatchManager::custom_device_mode_names_by_model
(
    const std::string & modelname
)
{
    if (! modelname.empty())
    {
        if (master_device_by_model(modelname))
        {
            return master_device_by_model(modelname)->
                custom_device_mode_names();
        }
    }
    return CustomDeviceModeNames();
}

/**
 *  For each device in the device-names list, if not already in the
 *  documents:
 *
 *      -   
 *
 */

bool
MidiPatchManager::add_midi_name_document (MIDINameDocumentPtr document)
{
    bool added { false };

#if 0
    for
    (
        DeviceNamesListEx::const_iterator device
        {
             document->master_device_names_by_model().begin()
        };
        device != document->master_device_names_by_model().end();
         ++device
    )
#endif

    for (const auto & device : document->master_device_names_by_model())
    {
        if (m_documents.find(device.first) != m_documents.end())
        {
            std::string msg
            {
                util::string_format
                (
                    "Duplicate MIDI device '%1' in '%2' ignored",
                    V(device.first), V(document->file_path())
                )
            };
            std::cerr << msg << std::endl;
            continue;
        }
        m_documents[device.first] = document;
        m_master_devices_by_model[device.first] = device.second;
        (void) m_all_models.insert(device.first);

        const std::string & manufacturer { device.second->manufacturer() };
        bool not_found
        {
            m_devices_by_manufacturer.find(manufacturer) ==
                m_devices_by_manufacturer.end()
        };
        if (not_found)
        {
            /* MIDINameDocument:: */

            MasterDeviceNamesList emptylist;
            m_devices_by_manufacturer.insert
            (
                std::make_pair(manufacturer, emptylist)
            );
        }
        m_devices_by_manufacturer[manufacturer].insert
        (
            std::make_pair(device.first, device.second)
        );
        added = true;

        // TODO: handle this gracefully.
        //
        //      assert(m_documents.count(device->first) == 1);
        //      assert(m_master_devices_by_model.count(device->first) == 1);
    }

    if (added && ! m_no_patch_changed_messages)
    {
        // PatchesChanged(); /* EMIT SIGNAL */
    }
    return added;
}

/**
 *  For each MIDINameDocument in the collection that matches the file-path,
 *  document is erased from the stored list of documents. Then for
 *  each device name ...
 *
 *  The emit_signal boolean parameter has been removed.
 */

bool
MidiPatchManager::remove_midi_name_document (const std::string & file_path)
{
    bool removed { false };
    for
    (
        MIDINameDocuments::iterator i = m_documents.begin();
        i != m_documents.end(); // ++i
    )
    {
        if (i->second->file_path() == file_path)
        {
            MIDINameDocumentPtr document { i->second };
            std::cout
                << "Removing MIDI patch file %1" << file_path
                << std::endl
                ;

            m_documents.erase(i++);
            for
            (
                /* MIDINameDocument:: */

                MasterDeviceNamesList::const_iterator device
                {
                    document->master_device_names_by_model().begin()
                };
                device != document->master_device_names_by_model().end();
                ++device
            )
            {
                m_master_devices_by_model.erase(device->first);
                m_all_models.erase(device->first);

                const std::string & manufacturer
                {
                    device->second->manufacturer()
                };
                m_devices_by_manufacturer[manufacturer].erase(device->first);
            }
            removed = true;
        }
        else
            ++i;
    }
    //  if (removed && emit_signal)
    //      PatchesChanged(); /* EMIT SIGNAL */

    return removed;
}

/**
 *  Really, there's only going to be one x-thread request/signal before
 *  this thread exits but we'll say 8 just to be sure.
 *
 *  We've removed the usage of pthreads and signals. Let the caller
 *  implement them
 */

void
MidiPatchManager::load_midnams ()
{
    for (auto p : search_paths().paths())
        add_midnam_files_from_directory(p);
}

#if 0

void
MidiPatchManager::load_midnams_in_thread ()
{
    if (! getenv("ARDOUR_NO_PATCHFILES"))
    {
        m_midnam_load_thread = PBD::Thread::create
        (
            std::bind(&MidiPatchManager::load_midnams, this),
            "MIDNAMLoader"
        );
    }
}

void
MidiPatchManager::maybe_use
(
    PBD::ScopedConnectionList & cl,
    PBD::EventLoop::InvalidationRecord* ir,
    const std::function<void()>  & midnam_info_method,
    PBD::EventLoop* event_loop
)
{
    Glib::Threads::Mutex::Lock lm (m_lock);
    if (!_documents.empty()) {
        /* already have documents loaded, so call closure to use them */
        midnam_info_method ();
    }

    /* if/when they ever change, call the closure (maybe multiple times) */

    PatchesChanged.connect (cl, ir, midnam_info_method, event_loop);
}

#endif

}           // namespace nam

}           // namespace midi

/*
 * midipatchmanager.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
