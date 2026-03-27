#*****************************************************************************
# meson.build (midiname/include)
#-----------------------------------------------------------------------------
##
# \file        include/meson.build
# \library     midiname
# \author      Chris Ahlstrom
# \date        2026-02-21
# \updates     2026-03-27
# \license     $XPC_SUITE_GPL_LICENSE$
#
#  This file is part of the "midiname" library. See the top-level meson.build
#  file for license information.
#
#  Here we locate the headers. The root of the header directory is 'include'.
#
#-----------------------------------------------------------------------------

libmidiname_headers += files(
   'midiname.hpp',
   'midi/nam/midnam_entities.hpp',
   'midi/nam/midnampack.hpp',
   'midi/nam/midipatchmanager.hpp',
   'midi/pack/channelnameset.hpp',
   'midi/pack/controlchange.hpp',
   'midi/pack/control.hpp',
   'midi/pack/customdevicemode.hpp',
   'midi/pack/extendingdevicenames.hpp',
   'midi/pack/masterdevicenames.hpp',
   'midi/pack/midicommands.hpp',
   'midi/pack/midinamedocument.hpp',
   'midi/pack/models.hpp',
   'midi/pack/notenamelist.hpp',
   'midi/pack/patchbank.hpp',
   'midi/pack/value.hpp'
   )

configure_file(
   input : 'midiname-config.h.in',
   output : 'midiname-config.h',
   configuration : cdata
   )

#****************************************************************************
# meson.build (midiname/include)
#----------------------------------------------------------------------------
# vim: ts=3 sw=3 ft=meson
#----------------------------------------------------------------------------
