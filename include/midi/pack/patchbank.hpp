#if ! defined MIDINAME_MIDI_PACK_PATCHBANK_HPP
#define MIDINAME_MIDI_PACK_PATCHBANK_HPP

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
 * \file          patchbank.hpp
 *
 *  Provides data structures mirroring some of the classes in the
 *  midnam_entities module.
 *
 * \library       midiname library
 * \author        Chris Ahlstrom
 * \date          2026-03-15
 * \updates       2026-03-16
 * \version       $Revision$
 *
 *  This module contains the midi::pack classes related to patches.
 *  No XML code is involved; no pointers are involved.
 */

#include <cstdint>                      /* std::uint16_t, uint8_t           */
#include <list>                         /* std::list<>                      */
#include <map>                          /* std::map<>                       */
#include <string>                       /* std::string<>                    */

namespace midi
{

namespace pack
{

// using PatchConstPtr     = std::shared_ptr<const Patch>;
// using PatchList = std::list<key>;
// using PatchMap          = std::map<key, PatchPtr>;
// using PatchNameList     = std::list<PatchPtr>;
// using PatchNameLists    = std::map<std::string, PatchNameList>;
// using PatchPtr          = std::shared_ptr<Patch>;

/**
 *  Patch
 */

class patch
{

public:

    // typedef std::list<std::shared_ptr<Patch> > PatchNameList;
    // using pointer = std::shared_ptr<Patch>;
    // using list = std::list<pointer>;

    using namelist = std::list<patch>;
    using banknumber = std::uint16_t;
    using prognumber = std::uint8_t;

    static const banknumber c_bank_max { 16383 };     /* 128x128-1, re 0   */
    static const prognumber c_program_max   {   127 };     /* 128-1, re 0       */

    class key
    {

    private:

        banknumber m_bank  { 0 };
        prognumber m_program    { 0 };

    public:

        key () = default;
        key (int programno, int bankno = 0);
        key (const key & id) = default;
        key & operator = (const key & id) = default;
        key (key && id) = default;
        key & operator = (key && id) = default;
        ~key () = default;

        bool operator == (const key & id) const;
        bool operator < (const key & id) const;

        void set_bank (int bank);
        void set_program (int program);

        banknumber bank () const
        {
            return m_bank;
        }

        prognumber program () const
        {
            return m_program;
        }

    };          // nested class key

private:

    /**
     *  The "Name" value of the "<Patch>" item.
     */

    std::string m_name { };

    /**
     *  Need to investigate this one.
     */

    std::string m_note_list_name { };

    /**
     *  Contains the bank and program numbers used to sort the patches.
     *
     *  The patch number is the "Number" value of the "<Patch>" item.
     *  The "ProgramChange" value seems to be the same as the "Number".
     *  It is not always present; and in some cases the "Number" is
     *  3 digits ending in a plus sign (see Roland_SC_88_Pro.midnam).
     *  What's up with that? Are the two states correlated?
     */

    key m_id { };

public:

    patch () = default;
    patch
    (
        const std::string & pname,
        prognumber programno            = 0,
        banknumber bankno          = 0
    );
    patch (const patch & id) = default;
    patch & operator = (const patch & id) = default;
    patch (patch && id) = default;
    patch & operator = (patch && id) = default;
    virtual ~patch () = default;

    const std::string & name () const
    {
        return m_name;
    }

    void set_name (const std::string & name)
    {
        m_name = name;
    }

    const std::string & note_list_name() const
    {
        return m_note_list_name;
    }

    prognumber program () const
    {
        return m_id.program();
    }

    void set_program (prognumber n)
    {
        m_id.set_program(n);
    }

    banknumber bank () const
    {
        return m_id.bank();
    }

    void set_bank (banknumber n)
    {
        m_id.set_bank(n);
    }

    const key & patch_primary_key () const
    {
        return m_id;
    }

//  xml66::XMLNode & get_state () const;
//  int set_state (const xml66::XMLTree &, const xml66::XMLNode &);

};          // class patch

/**
 *  patchbank
 */

class patchbank
{

private:

    std::string m_name { };
    patch::banknumber m_number { UINT16_MAX };  /* some don't have a number */
    patch::namelist m_patch_name_list { };      /* PatchNameList            */
    std::string m_patch_list_name { };

public:

    patchbank () = default;
    patchbank
    (
        patch::banknumber n,
        const std::string & aname = ""
    );
    patchbank (const patchbank & id) = default;
    patchbank & operator = (const patchbank & id) = default;
    patchbank (patchbank && id) = default;
    patchbank & operator = (patchbank && id) = default;
    virtual ~patchbank () = default;

    const std::string & name () const
    {
        return m_name;
    }

    void set_name(const std::string & aname)
    {
        m_name = aname;
    }

    int prognumber () const
    {
        return int(m_number);
    }

    const patch::namelist & patch_name_list () const
    {
        return m_patch_name_list;
    }

    const std::string & patch_list_name () const
    {
        return m_patch_list_name;
    }

    int set_patch_name_list (const patch::namelist &);

//  xml66::XMLNode & get_state () const;
//  int set_state (const xml66::XMLTree &, const xml66::XMLNode &);

};          // class patchbank

// using patchbankptr  = std::shared_ptr<patchbank>;
// using patchbanks    = std::list<patchbankptr>;

}           // namespace pack

}           // namespace midi

#endif      // MIDINAME_MIDI_PACK_PATCHBANK_HPP

/*
 * patchbank.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
