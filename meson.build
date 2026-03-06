#*****************************************************************************
# meson.build (midiname)
#-----------------------------------------------------------------------------
##
# \file        meson.build
# \library     midiname
# \author      Chris Ahlstrom
# \date        2026-02-21
# \updates     2026-02-25
# \license     $XPC_SUITE_GPL_LICENSE$
#
#  This file is part of the "midiname" library. It was part of the libs66
#  collection of libraries, but is now separate and stands alone.
#
#  The "midiname" library is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by the
#  Free Software Foundation, either version 3 of the License, or (at your
#  option) any later version.
#
#  The "midiname" library is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
#  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
#  more details.
#
#  You should have received a copy of the GNU General Public License along with
#  the "midiname" library. If not, see <https://www.gnu.org/licenses/>.
#
#     'warning_level=everything' is way too much.
#
#-----------------------------------------------------------------------------

project('midiname',
   [ 'c', 'cpp' ],
   version : '0.1',
   license : 'GPLv3',
   meson_version : '>=1.1.0',
   default_options : [
      'c_std=c11', 'cpp_std=c++17', 'warning_level=3',
      'default_library=both'
      ]
   )

#-----------------------------------------------------------------------------
# Conditional enabling of the Potext translation library.
#-----------------------------------------------------------------------------

midiname_use_potext = false
if get_option('potext')

   midiname_use_potext = true
   add_project_arguments('-DMIDINAME_USE_POTEXT', language : [ 'c', 'cpp' ])

endif

#-----------------------------------------------------------------------------
# Information for this sub-project.
#-----------------------------------------------------------------------------
#
#  Tricky: the *.pc file must have a base name matching the name of the
#  subproject *.wrap file.
#
#-----------------------------------------------------------------------------

midiname_pkg_description = 'Refactors base XML configuration/utility modules.'
midiname_pkg_version = meson.project_version()
midiname_api_version = '0.1'
midiname_info_date = '2026-02-25'
midiname_info_build_type = get_option('buildtype')
midiname_info_build_root = meson.project_build_root()
midiname_info_lib_type = get_option('default_library')
midiname_info_project_root = meson.project_source_root()
midiname_debug = get_option('buildtype').contains('debug')
midiname_project_base = 'midiname-@0@'.format(midiname_api_version)
midiname_pkgconfig_name = 'libmidiname'
midiname_subproj = meson.is_subproject()

# -fno-inline-functions does not work.

if midiname_debug
   if not midiname_subproj
      add_project_arguments(
         [ '-DDEBUG', '-fno-inline-functions' ],
         language : 'cpp'
         )
   endif
endif

#-----------------------------------------------------------------------------
# Easy access to directory options. Commented out the unnecessary ones.
# Many of these definitions are simply informative.
#-----------------------------------------------------------------------------

midiname_dir = 'midiname-@0@'.format(midiname_api_version)
midiname_prefix = get_option('prefix')
midiname_bindir = join_paths(midiname_prefix, get_option('bindir'))
midiname_includedir = join_paths(midiname_prefix, get_option('includedir'), midiname_dir)
midiname_libdir = join_paths(midiname_prefix, get_option('libdir'), midiname_dir)
midiname_datadir = join_paths(midiname_prefix, get_option('datadir'), midiname_dir)
midiname_docdir = join_paths(midiname_prefix, midiname_datadir, 'doc', midiname_dir)
libraries_top = meson.project_source_root()
libmidiname_include_top = join_paths(libraries_top, 'include')

#-----------------------------------------------------------------------------
# The *.pc file goes into $libdir/midiname-0.x/pkgconfig instead of
# $libdir/pkgconfig as the meson documentation asserts.  Not sure why, but
# wiring the expected directory here works.
#-----------------------------------------------------------------------------

alt_pkgconfig_libdir = join_paths(
   midiname_prefix, get_option('libdir'), 'pkgconfig'
   )

#-----------------------------------------------------------------------------
# libmidiname_headers provides the list of all headers in the project. These are
# currently all installable, public headers. They are set in
# include/meson.build.
#
# libmidiname_sources provides the source-code files. They are set in
# src/meson.build.
#-----------------------------------------------------------------------------

libmidiname_headers = []
libmidiname_sources = []
build_args = []

#-----------------------------------------------------------------------------
# Each include file directory is added to the list passed to the compiler.
# We prefer to use the style '#include "cfg/header.hpp".
#-----------------------------------------------------------------------------

libmidiname_includes = include_directories('.',    # libmidiname_include_top
   'include',
   'include/midinam'
   )

#-----------------------------------------------------------------------------
# Used by the midiname_version() function.
#-----------------------------------------------------------------------------

build_args += [
   '-DMIDINAME_NAME=' + '"' + meson.project_name() + '"',
   '-DMIDINAME_VERSION=' + '"' + meson.project_version() + '"'
   ]

#-----------------------------------------------------------------------------
# Only make public interfaces visible
#-----------------------------------------------------------------------------
#
# if target_machine.system() == 'windows' or target_machine.system() == 'cygwin'
#    build_args += '-DLIBPUBLIC="__declspec(dllexport)"'
# else
#    build_args += '-DLIBPUBLIC=__attribute__((visibility("default")))'
# endif

#-----------------------------------------------------------------------------
# Set up some "HAVE" macros.  By default the header file is generated in the
# "build" directory. If configure_file() is moved to include/meson.build,
# the the header goes into "build/include", which is what we want.
#-----------------------------------------------------------------------------

cc = meson.get_compiler('cpp')
cdata = configuration_data()
cdata.set10('limits_h', cc.has_header('limits.h'))

#-----------------------------------------------------------------------------
# Potential sub-projects
#-----------------------------------------------------------------------------

if midiname_use_potext

   libpotext_dep = subproject('libpotext').get_variable('libpotext_dep')
   libpotext_library_dep = dependency(
      'libpotext', fallback : [ 'libpotext', 'libpotext_dep' ]
      )

endif

liblib66_dep = subproject('liblib66').get_variable('liblib66_dep')
liblib66_library_dep = dependency(
   'liblib66', fallback : [ 'liblib66', 'liblib66_dep' ]
   )

libcfg66_dep = subproject('libcfg66').get_variable('libcfg66_dep')
libcfg66_library_dep = dependency(
   'libcfg66', fallback : [ 'libcfg66', 'libcfg66_dep' ]
   )

libxml66_dep = subproject('libxml66').get_variable('libxml66_dep')
libxml66_library_dep = dependency(
   'libxml66', fallback : [ 'libxml66', 'libxml66_dep' ]
   )

#-----------------------------------------------------------------------------
# Dependencies when building the libmidiname library.
#
# xmlxx_dep (created in libxml++/meson.build):
#
#   Dependencies when using the libxml++ library.
#-----------------------------------------------------------------------------

'''
xml2_min_ver = '2.9.0'
xml2_req = '>= @0@'.format(xml2_min_ver)
xml2_dep = dependency(
   ['libxml-2.0', 'LibXml2'],
   version: xml2_req,
   fallback: ['libxml2']
)
'''

xmlxx_pc_requires = []
libxml2_lib_pkgconfig = []

#-----------------------------------------------------------------------------
# Put libxml-2.0 in the 'Requires:' section in the generated pkg-config file if
# we found it by pkg-config. Then make sure we link to libxml-2.0.
#-----------------------------------------------------------------------------

'''
if xml2_dep.type_name() == 'pkgconfig' or xml2_dep.type_name() == 'internal'
   xmlxx_pc_requires += ['libxml-2.0' + xml2_req]
else
   libxml2_lib_pkgconfig += [xml2_dep.get_variable(
      cmake:         'LIBXML2_LIBRARIES',
      default_value: 'LibXml2.lib',
  )]
endif

xmlxx_build_dep = [xml2_dep]   # FIXME
'''

#-----------------------------------------------------------------------------
# Sub-directories. 'tests' has to be descended into later.
#-----------------------------------------------------------------------------

subdir('include')
subdir('src')

#-----------------------------------------------------------------------------
# Dependencies on Linux
#-----------------------------------------------------------------------------
#
# None, yet.
#
#-----------------------------------------------------------------------------

empty_depends = [ ]

#-----------------------------------------------------------------------------
# We recommemd using a recent version of meson by installing it outside the
# Linux distro's repository.  For example, meson on Ubuntu is pretty damn
# old.
#
# The choice of library type is of some import for debugging:
#
#     library:          Defaults to a shared library. The User setting is
#                       'default_library'.  See project() above.
#     shared_library:   Cannot see source files until stepped into.
#     static_library:   Can see the source files when debugging.
#
# This one causes the linker to not see the library functions, wotta pain:
#
#     gnu_symbol_visibility: 'hidden'
#
# Restore it later. Also see the https://gcc.gnu.org/wiki/Visibility site.
#
#-----------------------------------------------------------------------------

if midiname_use_potext

   midiname_library_build = library(
      midiname_project_base,
      libmidiname_sources,
      install : not midiname_subproj,
      install_dir : midiname_libdir,
      c_args : build_args,
      cpp_args : build_args,
      dependencies : [
         liblib66_library_dep, libcfg66_library_dep,
         libxml66_library_dep, libpotext_library_dep
         ],
      include_directories : [ libmidiname_includes ]
      )

else

# xm2_dep?

   midiname_library_build = library(
      midiname_project_base,
      libmidiname_sources,
      install : not midiname_subproj,
      install_dir : midiname_libdir,
      c_args : build_args,
      cpp_args : build_args,
      dependencies : [
         liblib66_library_dep, libcfg66_library_dep,
         libxml66_library_dep
         ],
      include_directories : [ libmidiname_includes ]
      )

endif

#-----------------------------------------------------------------------------
# Make this library usable as a Meson subproject.  This allows a subproject to
# easily specify how it should be used. This makes it interchangeable with the
# same dependency that is provided externally (i.e. installed) by the system.
#-----------------------------------------------------------------------------

libmidiname_dep = declare_dependency(
   include_directories : [ libmidiname_includes ],
   link_with : [ midiname_library_build ]
   )

#  system_depends = [ libmidiname_dep ]
#  dependencies : [ libxml2_dep ]

#-----------------------------------------------------------------------------
# Make this library usable from the system's package manager.
#-----------------------------------------------------------------------------

install_headers(libmidiname_headers, subdir : midiname_dir, preserve_path : true)

#-----------------------------------------------------------------------------
# Package config
#-----------------------------------------------------------------------------
#  On Arch Linux, the .pc file is installed to /usr/local/lib/pkgconfig.
#  On Ubuntu, the .pc file is installed to
#     /usr/local/lib/x86_64-linux-gnu/pkgconfig.
#-----------------------------------------------------------------------------

if not midiname_subproj

   pkg_mod = import('pkgconfig')
   pkg_mod.generate(
      midiname_library_build,
      filebase : midiname_pkgconfig_name,
      version : midiname_pkg_version,
      name : midiname_project_base,
      description : midiname_pkg_description,
      install_dir : alt_pkgconfig_libdir,
      subdirs : midiname_dir,
      libraries : midiname_library_build,
      url : 'https://github.com/ahlstromcj/midiname',
      )

# TODO: figure out where to put and install the logo

   install_data(
      files(
         'ChangeLog',
         'LICENSE.md',
         'README.md',
         'doc/midiname-library-guide.pdf'
         ),
      install_dir : get_option('docdir')
      )

   if get_option('enable-tests')
      subdir('tests')
   endif

endif

#-----------------------------------------------------------------------------
# Info
#-----------------------------------------------------------------------------

summary(
   {
      'Date' : midiname_info_date,
      'Filebase' : midiname_project_base,
      'Version' : midiname_pkg_version,
      'Base' : midiname_project_base,
      'Description' : midiname_pkg_description,
      'Source Root' : midiname_info_project_root,
      'Headers Root' : libmidiname_include_top,
      'Sub Dir' : midiname_project_base,
      'Build Type' : midiname_info_build_type,
      'Build Root' : midiname_info_build_root,
      'Library Type' : midiname_info_lib_type,
      'Subproject' : midiname_subproj
   },
   section : 'Midiname Project'
   )

summary(
   {
      'prefix' : midiname_prefix,
      'includedir' : midiname_includedir,
      'bindir' : midiname_bindir,
      'libdir' : midiname_libdir,
      'datadir' : midiname_datadir,
      'docdir' : midiname_docdir
   },
   section : 'Midiname Install Directories'
   )

#****************************************************************************
# meson.build (midiname)
#----------------------------------------------------------------------------
# vim: ts=3 sw=3 ft=meson
#----------------------------------------------------------------------------
