#
# Copyright (c) 2017 Enrico M. Crisostomo
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
ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = std-options

SUBDIRS = po

# Refresh configuration if libtool dependencies change
LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) $(top_builddir)/config.status libtool

# binaries
bin_PROGRAMS = display-control
display_control_SOURCES  = src/display-control.cpp src/display-control.h
display_control_SOURCES += src/gettext.h
display_control_SOURCES += src/gettext_defs.h
display_control_SOURCES += src/display.cpp src/display.h
display_control_SOURCES += src/CFTypeGuard.cpp src/CFTypeGuard.h
display_control_SOURCES += src/object-guard.h

display_control_LDFLAGS  = -framework IOKit
display_control_LDFLAGS += -framework ApplicationServices

# Link against libintl if gettext is being used
display_control_LDADD = @LTLIBINTL@

# distributed files
dist_doc_DATA  = README.codestyle
dist_doc_DATA += README.gnu-build-system
dist_doc_DATA += README.md
dist_doc_DATA += ABOUT-NLS AUTHORS CONTRIBUTING.md COPYING LICENSE NEWS

dist-hook:
# The recipe cannot assume a file in $(distdir) is writable.
	chmod u+w $(distdir)/ChangeLog
	git log --oneline --decorate >> $(distdir)/ChangeLog

# Manually distributing config.rpath seems not to be required any longer with
# current Autotools.
#
# EXTRA_DIST = config/config.rpath
