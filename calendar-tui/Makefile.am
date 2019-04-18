sbin_PROGRAMS += calendar-tui/calendar-tui

calendar_tui_calendar_tui_SOURCES = \
    calendar-tui/calendar-tui.c \
    calendar-tui/calendar.c \
    calendar-tui/calendar.h

calendar_tui_calendar_tui_LDADD = \
    -lncursesw -lformw \
    common/libcommon.la \
    boxes/libboxes.la \
    menus/libmenus.la

calendar_tui_calendar_tui_CFLAGS = \
     $(TUI_CFLAGS) \
    -DNCURSES_WIDECHAR \
    -I$(top_srcdir)/calendar-tui \
    -I$(top_srcdir)/boxes \
    -I$(top_srcdir)/menus \
    -I$(top_srcdir)/common \
    -Wall -ansi -pedantic -std=gnu99 -O3 -g
