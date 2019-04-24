/*
 * Copyright (c) 2019 Tibor Bodnar <piratetibi@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 2 as published
 * by the Free Software Foundation, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lessert General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <wchar.h>
#include <signal.h>
#include <locale.h>

#include "settings.h"
#include "calendar.h"
#include "file-handler.h"
#include "database.h"
#include "daydata.h"

#include "menus/menu.h"
#include "menus/box-menu.h"
#include "boxes/menu-box.h"

#define DAYS_DEF 7
#define WEEKS_DEF 4

Calendar *calendar;
DataBase *db;
FileHandler *dbfile;
int run;

static void move_page_up(Calendar *calendar, int weeks)
{
  int offset;
  offset = calendar_get_week_offset(calendar);
  offset -= weeks;
  calendar_set_week_offset(calendar, offset);
}

static void move_page_down(Calendar *calendar, int weeks)
{
  int offset;
  offset = calendar_get_week_offset(calendar);
  offset += weeks;
  calendar_set_week_offset(calendar, offset);
}

static void move_up(Calendar *calendar)
{
  int row, col;
  calendar_get_selected(calendar, &row, &col);

  if (row > 0)
    {
      calendar_set_selected(calendar, --row, col);
    }
  else
    {
      move_page_up(calendar, 1);
    }
  calendar_set_selected(calendar, row, col);
}

static void move_down(Calendar *calendar)
{
  int row, col, weeks;
  calendar_get_selected(calendar, &row, &col);
  weeks = calendar_get_weeks_num(calendar);

  if (row < weeks - 1)
    {
      calendar_set_selected(calendar, ++row, col);
    }
  else
    {
      move_page_down(calendar, 1);
    }
  calendar_set_selected(calendar, row, col);
}

static void move_right(Calendar *calendar)
{
  int row, col, days, weeks;
  calendar_get_selected(calendar, &row, &col);
  days = calendar_get_days_num(calendar);
  weeks = calendar_get_weeks_num(calendar);

  if (col < days - 1)
    {
      col++;
    }
  else
    {
      if (col == (days - 1) && (row == (weeks - 1)))
        {
          col = 0;
          move_down(calendar);
        }
      else
        {
          col = 0;
          row++;
        }
    }
  calendar_set_selected(calendar, row, col);
}

static void move_left(Calendar *calendar)
{
  int row, col, days;
  days = calendar_get_days_num(calendar);
  calendar_get_selected(calendar, &row, &col);

  if (col > 0)
    {
      col--;
    }
  else
    {
      if (col == 0 && row == 0)
        {
          col = days - 1;
          row = 0;
          move_up(calendar);
        }
      else
        {
          col = days - 1;
          row--;
        }
    }
  calendar_set_selected(calendar, row, col);
}

static void init_ncurses()
{
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  initscr();
  refresh();
}

static void init_colors()
{
  start_color();
  init_pair(C_HEADER_BACKGROUND, COLOR_GREEN, COLOR_BLACK);
  init_pair(C_HEADER_TITLE, COLOR_GREEN, COLOR_BLACK);
  init_pair(C_CELL_BORDER, COLOR_WHITE, COLOR_BLACK);
  init_pair(C_CELL_WEEKDAY, COLOR_GREEN, COLOR_BLACK);
  init_pair(C_CELL_WEEKDAY_LABEL, COLOR_WHITE, COLOR_BLACK);
  init_pair(C_CELL_WEEKEND, COLOR_GREEN, COLOR_BLACK);
  init_pair(C_CELL_WEEKEND_LABEL, COLOR_WHITE, COLOR_GREEN);
  init_pair(C_CELL_SELECTED, COLOR_BLUE, COLOR_WHITE);
  init_pair(C_CELL_SELECTED_LABEL, COLOR_BLUE, COLOR_WHITE);
  init_pair(C_CELL_TODAY, COLOR_WHITE, COLOR_BLACK);
  init_pair(C_CELL_TODAY_LABEL, COLOR_WHITE, COLOR_RED);
  init_pair(C_HOLIDAY, COLOR_RED, COLOR_BLACK);
  init_pair(C_MENUBAR, COLOR_WHITE, COLOR_BLUE);
  init_pair(C_MENUBAR_LABEL, COLOR_BLUE, COLOR_WHITE);
  init_pair(C_RANGE, COLOR_BLACK, COLOR_RED);
  init_pair(C_RANGE_INV, COLOR_RED, COLOR_BLACK);
  init_pair(C_POPUP, COLOR_BLACK, COLOR_RED);

  init_pair(C_VACATION, COLOR_GREEN, COLOR_BLACK);
  init_pair(C_SICK_LEAVE, COLOR_GREEN, COLOR_BLACK);
  init_pair(C_NOT_WORKING, COLOR_GREEN, COLOR_BLACK);
  init_pair(C_NON_PAID_HOLIDAY, COLOR_GREEN, COLOR_BLACK);

  init_pair(C_HOME_OFFICE, COLOR_YELLOW, COLOR_GREEN);
  init_pair(C_STANDBY, COLOR_BLUE, COLOR_GREEN);

  init_pair(C_CONFERENCE, COLOR_YELLOW, COLOR_BLACK);
  init_pair(C_TRAINING, COLOR_YELLOW, COLOR_BLACK);
  init_pair(C_BUSINESS_TRIP, COLOR_YELLOW, COLOR_BLACK);

  init_pair(C_LIFE_LONG_LEARNING, COLOR_BLACK, COLOR_RED);

  init_pair(C_HALF_HOLIDAY_AM, COLOR_BLACK, COLOR_GREEN);
  init_pair(C_HALF_HOLIDAY_PM, COLOR_BLACK, COLOR_GREEN);

  init_pair(C_DEFAULT, COLOR_RED, COLOR_GREEN);
}

static void
clear_screen()
{
  bkgdset(' ');
  clear();
  refresh();
}

static int hup_signal_received = FALSE;

static void sig_hup_handler(int signo)
{
  hup_signal_received = TRUE;
}

static void init_sig_handlers()
{
  struct sigaction sa;

  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = sig_hup_handler;
  sigaction(SIGHUP, &sa, NULL);
}

void
_show_pop_up(char *message)
{
  int len;
  int height = 5;
  int width;
  int pos_x;
  int pos_y = (LINES - height) / 2;
  wchar_t *multibyte_message;
  HeaderBox *popup;

  if (!message)
    return;

  multibyte_message = malloc(strlen(message) * sizeof(wchar_t));
  mbstowcs(multibyte_message, message, strlen(message));
  len = wcslen(multibyte_message);
  width = len + 4;
  pos_x = (COLS - (len + 2)) / 2;

  popup = headerbox_new();

  headerbox_init(popup, height, width, pos_y, pos_x);
  headerbox_set_title(popup, multibyte_message);
  headerbox_set_title_color(popup, C_HEADER_TITLE);
  box_set_bg_color((Box *) popup, C_POPUP);
  box_draw((Box *) popup);

  timeout(5000);
  getch();
  timeout(500);

  box_free((Box *) popup);
  free(multibyte_message);
}

static int
check_signal(Calendar **calendar, DataBase **db, FileHandler **dbfile, MenuBox *menubox)
{
  char *alert_message;

  if (!hup_signal_received)
    {
      return -1;
    }

  hup_signal_received = FALSE;

  calendar_free(*calendar);
  filehandler_free(*dbfile);
  database_free(*db);

  *db = database_new();
  *dbfile = filehandler_new(*db, "db/");
  filehandler_read_files(*dbfile);

  *calendar = calendar_new();
  calendar_init(*calendar, *db, 0, 1, DAYS_DEF, WEEKS_DEF);
  menubox_call(menubox);

  alert_message = database_get_alert(*db);
  if (alert_message)
    {
      _show_pop_up(alert_message);
    }

  menubox_call(menubox);

  return 0;
}

void
_one_weekdays_calendar()
{
  calendar_free(calendar);
  calendar = calendar_new();
  calendar_init(calendar, db, 0, 1, 5, 1);
  calendar_draw(calendar);
}

void
_two_weekdays_calendar()
{
  calendar_free(calendar);
  calendar = calendar_new();
  calendar_init(calendar, db, 0, 1, 5, 2);
  calendar_draw(calendar);
}

void
_four_weekdays_calendar()
{
  calendar_free(calendar);
  calendar = calendar_new();
  calendar_init(calendar, db, 0, 1, 5, 4);
  calendar_draw(calendar);
}

void
_two_weeks_calendar()
{
  calendar_free(calendar);
  calendar = calendar_new();
  calendar_init(calendar, db, 0, 1, 7, 2);
  calendar_draw(calendar);
}

void
_one_week_calendar()
{
  calendar_free(calendar);
  calendar = calendar_new();
  calendar_init(calendar, db, 0, 1, 7, 1);
  calendar_draw(calendar);
}

void
_one_month_calendar()
{
  calendar_free(calendar);
  calendar = calendar_new();
  calendar_init(calendar, db, 0, 1, 7, 4);
  calendar_draw(calendar);
}

int
main(int argc, char *argv[])
{
  BoxMenu *rangemenu = boxmenu_new();
  MenuBox *rangemenubox = menubox_new();

  run = 1;

  // make the program run with the user's locale
  // this is needed so that the wchar <-> local charset conversion is correct
  setlocale(LC_ALL, "");

  init_ncurses();
  init_colors();
  init_sig_handlers();

  db = database_new();
  dbfile = filehandler_new(db, "db/");
  filehandler_read_files(dbfile);

  calendar = calendar_new();
  calendar_init(calendar, db, 0, 1, DAYS_DEF, WEEKS_DEF);

  menubox_init(rangemenubox, 1, 0);
  boxmenu_init(rangemenu, rangemenubox, 1, 15, 0, 0, L"Range <1>");
  menu_set_bg_color((Menu *) rangemenu, C_MENUBAR);
  menu_set_txt_color((Menu *) rangemenu, C_MENUBAR_LABEL);
  box_set_bg_color((Box *) rangemenubox, C_RANGE);
  menubox_add_option(rangemenubox, L"Show one weekdays", _one_weekdays_calendar);
  menubox_add_option(rangemenubox, L"Show two weekdays", _two_weekdays_calendar);
  menubox_add_option(rangemenubox, L"Show four weekdays", _four_weekdays_calendar);
  menubox_add_option(rangemenubox, L"Show one week", _one_week_calendar);
  menubox_add_option(rangemenubox, L"Show two weeks", _two_weeks_calendar);
  menubox_add_option(rangemenubox, L"Show one month", _one_month_calendar);
  menubox_set_selected_color(rangemenubox, C_RANGE_INV);
  menubox_set_selected(rangemenubox, 5);

  clear_screen();
  calendar_draw(calendar);
  menu_draw((Menu *) rangemenu);

  timeout(500);

  while (run)
    {
      switch (getch())
        {
          case KEY_UP:
            move_up(calendar);
          break;
          case KEY_DOWN:
            move_down(calendar);
          break;
          case KEY_LEFT:
            move_left(calendar);
          break;
          case KEY_RIGHT:
            move_right(calendar);
          break;
          case KEY_PPAGE:
            move_page_up(calendar, SCROLL_WEEKS);
          break;
          case KEY_NPAGE:
            move_page_down(calendar, SCROLL_WEEKS);
          break;
          case 'h':
            calendar_hide_selected(calendar);
          break;
          case 'q':
            run = 0;
          break;
          case '1':
            menu_use((Menu *) rangemenu);
          break;
          case -1:
            if (check_signal(&calendar, &db, &dbfile, rangemenubox))
              continue;
          break;
        }

      clear_screen();
      calendar_draw(calendar);
      menu_draw((Menu *) rangemenu);
    }

  echo();
  endwin();

  calendar_free(calendar);
  filehandler_free(dbfile);
  database_free(db);

  menu_free((Menu *) rangemenu);

  return 0;
}
