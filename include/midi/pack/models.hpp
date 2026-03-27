#if ! defined MIDINAME_MIDI_PACK_MODELS_HPP
#define MIDINAME_MIDI_PACK_MODELS_HPP

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
 * \file          models.hpp
 *
 *  Provides data structures mirroring some of the classes in the
 *  midnam_entities module.
 *
 * \library       midiname library
 * \author        Chris Ahlstrom
 * \date          2026-03-26
 * \updates       2026-03-27
 * \version       $Revision$
 *
 *  This module contains the midi::pack classes related to modelses.
 *  No XML code is involved; no pointers are involved.
 *
 *          MIDNAM Element          midi::pack class
 *
 *      -   Model                   model
 *      -   <none>                  model::list
 *      -   <none>                  models
 *      -   Manufacturer            models::m_manufacturer
 */

#include <vector>                       /* std::vector<>                    */
#include <string>                       /* std::string<>                    */

namespace midi
{

namespace pack
{

/**
 *  Model. An element that simply holds a model name.
 */

class model
{

public:

    /**
     *  Holds a list of model (model names).
     */

    using list = std::vector<model>;

private:

    std::string m_name;

public:

    model () = default;
    model (const std::string & modelname);
    model (const model & id) = default;
    model & operator = (const model & id) = default;
    model (model && id) = default;
    model & operator = (model && id) = default;
    ~model () = default;

    const std::string & name () const
    {
        return m_name;
    }

    void name (const std::string & n)
    {
        m_name = n;
    }

};

/**
 *  models
 */

class models
{

private:

    /**
     *  Holds the Manufacturer name for MasterDeviceNames and
     *  ExtendingDeviceNames.
     */

    std::string m_manufacturer { };

    /**
     *  Holds the list of Models. There is either 1 or many model
     *  objects in this list. Used in MasterDeviceNames and
     *  ExtendingDeviceNames.
     */

    model::list m_model_list { };

public:

    models () = default;

    models (const std::string & manufname) :
        m_manufacturer (manufname)
    {
        // no code
    }

    models (const models & id) = default;
    models & operator = (const models & id) = default;
    models (models && id) = default;
    models & operator = (models && id) = default;
    ~models () = default;

    bool add_model (const std::string & m)
    {
        bool result { ! m.empty() };
        if (result)
        {
            model mdl(m);
            m_model_list.push_back(mdl);
        }
        return result;
    }

    void model_list (const model::list & ml)
    {
        m_model_list = ml;
    }

    model::list & model_list ()
    {
        return m_model_list;
    }

    const model::list & model_list () const
    {
        return m_model_list;
    }

    const std::string & manufacturer () const
    {
        return m_manufacturer;
    }

    void manufacturer (const std::string & m)
    {
        m_manufacturer = m;
    }

};          // class models

}           // namespace pack

}           // namespace midi

#endif      // MIDINAME_MIDI_PACK_MODELS_HPP

/*
 * models.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
