#                                               -*- Autoconf -*-
#
# Copyright (C) 2017 Enrico M. Crisostomo
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation; either version 3, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Process this file with autoconf to produce a configure script.
#
dnl Copyright (C) 2017 Enrico M. Crisostomo

AC_PREREQ([2.69])
m4_include([m4/display_control_version.m4])
AC_INIT([display-control], DISPLAY_CONTROL_VERSION, [enrico.m.crisostomo@gmail.com], [], [https://github.com/emcrisostomo/display-control])
AC_COPYRIGHT([2017 (C) Enrico M. Crisostomo])
AC_REVISION([$Revision: DISPLAY_CONTROL_REVISION$])
AC_CONFIG_SRCDIR([src/display-control.cpp])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADERS([display_control_config.h])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([po/Makefile.in])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_MACRO_DIR([m4])

# Compute the canonical target-system type variables
AC_CANONICAL_TARGET
TARGET_VENDOR=$target_vendor
TARGET_OS=$target_os
AC_SUBST([TARGET_VENDOR])
AC_SUBST([TARGET_OS])

# Initialize Automake.
AM_INIT_AUTOMAKE([-Wall -Werror gnu subdir-objects std-options 1.14])
AM_SILENT_RULES([yes])
AM_PROG_AR

# Configure language.
AC_LANG(C++)
AC_PROG_CXX([clang++ g++])
AC_PROG_INSTALL

# Initialize gettext.
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19.4])

# Configure C++ compiler
AX_CXX_COMPILE_STDCXX_11
AX_CXXFLAGS_WARN_ALL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([unistd.h], [], [AC_MSG_ERROR([A required header file is missing.])])
AC_CHECK_HEADERS([CoreFoundation/CoreFoundation.h], [], [AC_MSG_ERROR([A required header file is missing.])])
AC_CHECK_HEADERS([IOKit/graphics/IOGraphicsLib.h], [], [AC_MSG_ERROR([A required header file is missing.])])
AC_CHECK_HEADERS([ApplicationServices/ApplicationServices.h], [], [AC_MSG_ERROR([A required header file is missing.])])

# Check for optional getopt_long
AC_CHECK_HEADERS([getopt.h])
AS_VAR_IF([ac_cv_header_getopt_h], ["yes"], [AC_CHECK_FUNCS([getopt_long])])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT64_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_CHECK_FUNCS([strtoul])

AC_OUTPUT
