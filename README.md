# README for Library Midiname 0.1 2026-03-06

__Midiname__ is a small XML support library based on the "66" projects.
It is meant primarily to create a MIDINAM implementation based on heavily
modified code from the Ardour and UtfCpp projects.

Support sites (still in progress):

    *   https://ahlstromcj.github.io/
    *   https://github.com/ahlstromcj/ahlstromcj.github.io/wiki

# Major Features

The "midiname" directory holds code for handling various tasks needed
for the use of XML. Midiname contains the following subdirectories,
each of which holds modules or classes in a namespace of the same
name:

    *   midinam Contains classes and code to handle items in a MIDINAM
                file.
    *   tests:  Small test applications.

    Note that a work.sh script is provided to simplify or clarify
    various operations such as cleaning, building, making a release,
    and installing or uninstalling the library.

##  Dependencies

    *   The lib66 project. It provides a few entities common to all
        "66" projects, such as the not_nullptr() macro and the
        lib66::tokenization vector of strings. If not installed, it is
        downloaded (from GitHub) and built with the rest of the code
        as a Meson subproject.
    *   The cfg66 project. It provides functions for handling strings,
        messages, files, file-tree walking, string manipulation, and
        more. It is also handled as a Meson subproject.
    *   The xml66 project. It provides a cut-down version of
        libxmlplusplus (xml++) providing just enough to support
        using MIDINAM files. It is also handled as a Meson subproject.
    *   The potext project. It provides an alternative to gettext,
        but is optional. It is also handled as a Meson subproject.
    *   libxml2. This 3rd-party library, including its devlopement
        package, must be installed. The xml66 subproject automatically
        brings it.

##  Library Features

    *   Can be built using GNU C++ or Clang C++.
    *   Basic dependencies: Meson 1.1 and above; C++17 and above;
        xml66 and libxml2.
    *   The build system is Meson, and sample wrap files are provided
        for using Midiname as a C++ subproject.
    *   PDF documentation built from LaTeX.

##  Code

    *   The code is a mix of hard-core C++ and C-like functions.
    *   The C++ STL and advanced language features are used as much as
        possible
    *   C++17 is required for some of its features.
    *   The GNU and Clang C++ compilers are supported.
    *   Broken into modules for easier maintenance.

##  Fixes

    *   Tbd.

##  Documentation

    *   A PDF developers guide will eventually be written.

## To Do

    *   Beef up testing.
    *   Beef up the LaTeX documentation.

## Recent Changes

    For all changes, see the NEWS file.

    *   Version 0.1:
        *   We begin!

// vim: sw=4 ts=4 wm=12 et ft=markdown
