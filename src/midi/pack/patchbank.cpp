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
 * \updates       2026-03-15
 * \version       $Revision$
 *
 *  This module contains the midi::pack classes related to patches.
 *  No XML code is involved; no pointers are involved.
 *
 *  Defines patch, patch::key, and patchbank.
 */

#include "midi/pack/patchbank.hpp"      /* midi::pack::patch & patchbank    */
#include <list>                         /* std::list<>                      */
#include <map>                          /* std::map<>                       */

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

    m_bank = bank_number(bank);
}

void
patch::key::set_program (int program)
{
    if (program < 0 || program > patch::c_program_max)
        program = 0;

    m_program = number(program);
}

/**
 *  class patch
 */

patch::patch
(
    const std::string & pname,
    number programno,
    bank_number bankno
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
    patch::bank_number n,
    const std::string & aname
) :
    m_name      (aname),
    m_number    (n)
{
    // no code
}

int
patchbank::set_patch_name_list (const patch::list & lst)
{
    m_patch_name_list = lst;
    m_patch_list_name.clear();
    for (auto & p : m_patch_name_list)
        p.set_bank(m_number);

    return 0;
}

}           // namespace pack

}           // namespace midi

/*
 * patchbank.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
