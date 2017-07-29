[![License](https://img.shields.io/badge/license-GPL--3.0-blue.svg?style=flat)](https://github.com/emcrisostomo/fswatch/blob/master/LICENSE)

README
======

`display-control` is a macOS command line tool to enumerate displays and store
and set their brightness settings.

Table of Contents
-----------------

  * [Features](#features)
  * [Limitations](#limitations)
  * [Building from Source](#building-from-source)
  * [Installation](#installation)
  * [Localization](#localization)
  * [Usage](#usage)
  * [Contributing](#contributing)
  * [Bug Reports](#bug-reports)

Features
--------

`display-control` main features are:

  * List displays.
  * Get the display brightness settings.
  * Update the display brightness settings.

Limitations
-----------

`display-control` may not work with external monitors, depending on their model
and interface type.

Building from Source
--------------------

A user who wishes to build `display-control` should get a
[release tarball][release].  A release tarball contains everything a user needs
to build `display-control` on their system, following the instructions detailed
in the Installation section below and the `INSTALL` file.

A developer who wishes to modify `display-control` should get the sources
(either from a source tarball or cloning the repository) and have the GNU Build
System installed on their machine.  Please read `README.gnu-build-system` to get
further details about how to bootstrap `display-control` from sources on your
machine.

Getting a copy of the source repository is not recommended unless you are a
developer, you have the GNU Build System installed on your machine, and you know
how to bootstrap it on the sources.

[release]: https://github.com/emcrisostomo/display-control/releases

Installation
------------

See the `INSTALL` file for detailed information about how to configure and
install `display-control`.  Since the `display-control` builds and uses dynamic
libraries, in some platforms you may need to perform additional tasks before you
can use `display-control`:

  * Make sure the installation directory of dynamic libraries (`$PREFIX/lib`) is
    included in the lookup paths of the dynamic linker of your operating system.
    The default path, `/usr/local/lib`, will work in nearly every operating
    system.

`display-control` is a C++ program and a C++ compiler compliant with the C++11
standard is required to compile it.  Check your OS documentation for information
about how to install the C++ toolchain and the C++ runtime.

No other software packages or dependencies are required to configure and install
`display-control` but the aforementioned APIs used by the file system monitors.

Localization
------------

`display-control` is localizable and internally uses GNU `gettext` to decouple
localizable string from their translation.  The currently available locales are:

  * English (`en`).
  * Italian (`it`).
  * Spanish (`es`).

To build `display-control` with localization support, you need to have `gettext`
installed on your system.  If `configure` cannot find `<libintl.h>` or the
linker cannot find `libintl`, then you may need to manually provide their
location to `configure`, usually using the `CPPFLAGS` and the `LDFLAGS`
variables.  See `README.osx` for an example.

If `gettext` is not available on your system, `display-control` shall build
correctly, but it will lack localization support and the only available locale
will be English.

Usage
-----

By default, unless a different operation is specified, `display-control` queries
the system to get the list of displays and their brightness settings:

    $ display-control
    0:0.417969

One row per display will be returned with this format:

    i:b

where `i` is the display index and `b` the current brightness value.

The brightness value can be changed using the `-b` option:

    $ display-control -b .5

Unless `-d` is used, the brightness settings of the first display will be
updated.  A list of displays to update can be specified using the `-d` option:

    $ display-control -b .5 -d 0 -d 1


Contributing
------------

Everybody is welcome to contribute to `display-control`.  Please, see
[`CONTRIBUTING`][contrib] for further information.

[contrib]: CONTRIBUTING.md

Bug Reports
-----------

Bug reports can be sent directly to the authors.

-----

Copyright (c) 2017 Enrico M. Crisostomo

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 3, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
