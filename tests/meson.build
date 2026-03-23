#*****************************************************************************
# meson.build (midiname/tests)
#-----------------------------------------------------------------------------
##
# \file        tests/meson.build
# \library     midiname
# \author      Chris Ahlstrom
# \date        2026-02-20
# \updates     2026-03-23
# \license     $XPC_SUITE_GPL_LICENSE$
#
#  This file is part of the "midiname" library. See the top-level meson.build
#  file for license information.
#
#-----------------------------------------------------------------------------

midiname_tests_exe = executable(
   'midiname_tests',
   sources : [ 'midiname_tests.cpp' ],
   dependencies : [
      libmidiname_dep, libcfg66_library_dep,
      libxml66_library_dep, liblib66_library_dep
      ]
   )

midnampack_tests_exe = executable(
   'midnampack_tests',
   sources : [ 'midnampack_tests.cpp' ],
   dependencies : [
      libmidiname_dep, libcfg66_library_dep,
      libxml66_library_dep, liblib66_library_dep
      ]
   )

mpatchmgr_tests_exe = executable(
   'mpatchmgr_tests',
   sources : [ 'mpatchmgr_tests.cpp' ],
   dependencies : [
      libmidiname_dep, libcfg66_library_dep,
      libxml66_library_dep, liblib66_library_dep
      ]
   )

test('C++ Midiname Tests', midiname_tests_exe)
test('C++ Midnampack Tests', midnampack_tests_exe)
test('C++ MidiPatchManager Tests', mpatchmgr_tests_exe)
   
#****************************************************************************
# meson.build (midiname/tests)
#----------------------------------------------------------------------------
# vim: ts=3 sw=3 ft=meson
#----------------------------------------------------------------------------
