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
