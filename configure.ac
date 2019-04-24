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

AC_INIT([calendar-tui], m4_esyscmd([tr -d '\n' < VERSION]))
AM_INIT_AUTOMAKE([foreign no-define subdir-objects])
AC_CONFIG_FILES([Makefile])

AC_SUBST([CC])
AC_PROG_CC

LT_INIT([dlopen disable-static])

AC_ARG_ENABLE(debug, [--enable-debug  Enable debugging code.],, enable_debug="no")

PKG_CHECK_MODULES(NCURSESW, ncursesw >= "6.0")

AC_DEFINE_UNQUOTED(ENABLE_DEBUG, `enable_value $enable_debug`, [Enable debugging])

TUI_CFLAGS="$NCURSESW_CFLAGS"

if test "x$enable_debug" = "xyes"; then
  TUI_CFLAGS="$TUI_CFLAGS -Wall -ggdb"
fi

AC_SUBST(TUI_CFLAGS)
AC_OUTPUT
echo
echo "TUI on TV $VERSION configured"
echo "Debugging:        ${enable_debug:=no}"
echo
