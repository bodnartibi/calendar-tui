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

lib_LTLIBRARIES += boxes/libboxes.la

boxes_libboxes_la_SOURCES = \
    boxes/box.c \
    boxes/day-box.c \
    boxes/header-box.c \
    boxes/menu-box.c \
    boxes/box.h \
    boxes/day-box.h \
    boxes/header-box.h \
    boxes/menu-box.h

boxes_libboxes_la_CFLAGS = \
    $(TUI_CFLAGS) \
    -I$(top_srcdir)/common
