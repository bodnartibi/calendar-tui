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
