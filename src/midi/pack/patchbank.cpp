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
 */

/**
 * \file          patchbank.cpp
 *
 *  Provides data structures mirroring some of the classes in the
 *  midnam_entities module.
 *
 * \library       midiname library
 * \author        Chris Ahlstrom
 * \date          2026-03-15
 * \updates       2026-03-23
 * \version       $Revision$
 *
 *  This module contains the midi::pack classes related to patches.
 *  No XML code is involved; no pointers are involved.
 *
 *  Defines patch, patch::key, and patchbank.
 *
 *  Layout notes:
 *
 *      Waldorf_Blofeld.midnam (from Ardour's patchfiles subdirectory):
 *
 *      MIDINAMEDocument
 *          Author
 *          MasterDeviceNames
 *              Manufacturer
 *              Model
 *              CustomDeviceMode
 *              ChannelNameSet
 *                  AvailableForChannels
 *                  UsesControlNameList
 *                  PatchBank
 *                      MIDICommands
 *                          ControlChange
 *                      UsesPatchNameList
 *              PatchNameList
 *                  Patch ...
 *
 *      Roland_MT_32.midnam (tests/data):
 *
 *                  PatchBank
 *                      PatchNameList
 *                          Patch ...
 *
 *      Yamaha_PSR_S900.midnam (tests/data):
 *
 *                  PatchBank
 *                      PatchNameList
 *                          Patch ...
 *                              PatchMIDICommands
 *                                  ControlChange x 2
 *                                  ProgramChange
 *
 *      Bitheadz_Retro_AS_1.midnam (from Ardour's patchfiles subdirectory):
 *
 *                  PatchBank
 *                      MIDICommands
 *                          ControlChange x 2
 *                      PatchNameList
 *                          Patch ...
 *
 *      E_mu_Systems_P2000_X_Lead.midnam or Kurzweil_SP88x.midnam:
 *
 *              ExtendingDeviceNames>
 *                  PatchNameList
 *                      Patch ...
 */

#include "midi/pack/patchbank.hpp"      /* midi::pack::patch & patchbank    */

namespace midi
{

namespace pack
{

/**
 *  class patch::key
 */

patch::key::key (int programno, int bankno)
{
    set_bank(bankno);
    set_program(programno);
}

bool
patch::key::operator == (const key & id) const
{
    return m_bank == id.m_bank && m_program == id.m_program;
}

/*
 *  Strict weak ordering.
 */

bool
patch::key::operator < (const key & id) const
{
    if (m_bank < id.m_bank)
        return true;
    else if (m_bank == id.m_bank && m_program < id.m_program)
        return true;

    return false;
}

void
patch::key::set_bank (int bank)
{
    if (bank < 0 || bank > patch::c_bank_max)
        bank = 0;

    m_bank = banknumber(bank);
}

void
patch::key::set_program (int program)
{
    if (program < 0 || program > patch::c_program_max)
        program = 0;

    m_program = prognumber(program);
}

/**
 *  class patch
 */

patch::patch
(
    const std::string & pname,
    prognumber programno,
    banknumber bankno
) :
    m_name  (pname),
    m_id    (programno, bankno)
{
    // no code
}

/**
 *  class patchbank
 */

patchbank::patchbank
(
    const std::string & name,
    bool hasrom,
    patch::banknumber n
) :
    m_name      (name),
    m_number    (n),
    m_has_rom   (hasrom)
{
    // no code
}

#if defined USE_PATCHBANK_PATCH_NAMELIST

int
patchbank::set_patch_name_list (const patch::namelist & lst)
{
    m_patch_name_list = lst;
    m_patch_list_name.clear();
    for (auto & p : m_patch_name_list)
        p.set_bank(m_number);

    return 0;
}

#endif

/**
 *  class patchbanks
 */

void
patchbanks::add (const patchbank & pb)
{
    patch_banks().push_back(pb);
}

/**
 *  This function makes it more convenient for callers to get the
 *  list of patchbank names.
 */

lib66::tokenization
patchbanks::names () const
{
    lib66::tokenization result;
    for (const auto & pb : patch_banks())
    {
        if (pb.name().empty())
        {
            result.clear();
            break;
        }
        else
            result.push_back(pb.name());
    }
    return result;
}

}           // namespace pack

}           // namespace midi

/*
 * patchbank.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
