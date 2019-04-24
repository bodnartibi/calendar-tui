#
# Copyright (c) 2019 Tibor Bodnar <piratetibi@gmail.com>
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License version 2 as published
# by the Free Software Foundation, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lessert General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#

lib_LTLIBRARIES += common/libcommon.la

common_libcommon_la_SOURCES = \
    common/database.c \
    common/database.h \
    common/daydata.c \
    common/daydata.h \
    common/file-handler.c \
    common/file-handler.h

common_libcommon_la_CFLAGS = \
    $(TUI_CFLAGS) \
    -DNCURSES_WIDECHAR \
    -I$(top_srcdir)/calendar-tui \
    -Wall -ansi -pedantic -std=gnu99 -O3 -g
