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
 *
 */

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "settings.h"
#include "calendar.h"
#include "box.h"
#include "header-box.h"
#include "day-box.h"
#include "database.h"

static struct tm *
_get_date_by_offset(int offset)
{
  time_t raw_time;

  time(&raw_time);
  raw_time += offset * 24 * 60 * 60;
  return localtime(&raw_time);
}

static void
_calendar_draw_cells(Calendar *self)
{
  if (self->boxes && self->dayboxes)
    {
      for (int cell_row = 0; cell_row < self->weeks; cell_row++)
        {
          for (int cell_col = 0; cell_col < self->days; cell_col++)
            {
              box_draw((Box *) (self->boxes[cell_row][cell_col]));
              box_draw((Box *) (self->dayboxes[cell_row][cell_col]));
            }
        }
    }
}

static void
_calendar_draw_header(Calendar *self)
{
  int day;
  if (self->headers)
    {
      for (day = 0; day < self->days; day++)
        {
          box_draw((Box *) self->headers[day]);
        }
    }
}

static void _draw(Calendar *self)
{
  _calendar_draw_header(self);
  _calendar_draw_cells(self);
}

static void _free(Calendar *self)
{
  int day, week;

  for (day = 0; day < self->days; day++)
    {
      box_free((Box *) self->headers[day]);
    }

  for (week = 0; week < self->weeks; week++)
    {
      for (day = 0; day < self->days; day++)
        {
          box_free((Box *) self->dayboxes[week][day]);
          box_free((Box *) self->boxes[week][day]);
        }
      free(self->dayboxes[week]);
      free(self->boxes[week]);
    }

  free(self->headers);
  free(self->dayboxes);
  free(self->boxes);
  free(self);
}

void
calendar_draw(Calendar *self)
{
  self->draw_fn(self);
}

static void
_calendar_init_headers(Calendar *self)
{
  HeaderBox *header;
  wchar_t *titles[] = {L"Hétfő", L"Kedd", L"Szerda", L"Csütörtök", L"Péntek", L"Szombat", L"Vasárnap"};

  int startx, starty;

  starty = self->pos_y;
  for (int day = 0; day < self->days; day++)
    {
      startx = (COLS - self->days * self->header_width) / 2 + day * self->header_width + self->pos_x;
      self->headers[day] = header = headerbox_new();
      headerbox_init(header, self->header_height, self->header_width + 1, starty, startx);
      box_set_borders((Box *) header, 0, 0, 0, 0, 0, 0, 0, 0);
      box_set_bg_color((Box *) header, C_HEADER_BACKGROUND);
      headerbox_set_title(header, titles[day]);
      headerbox_set_title_color(header, C_HEADER_TITLE);
    }
}

static void
_calendar_init_cells(Calendar *self)
{
  DayData *data;
  int day_of_week;
  int day_offset;
  struct tm *tm_info;

  tm_info = _get_date_by_offset(0 + self->week_offset * 7);
  day_of_week = tm_info->tm_wday - 1;

  Box *cell;
  DayBox *daycell;
  int startx, starty;
  for (int cell_row = 0; cell_row < self->weeks; cell_row++)
    {
      for (int cell_col = 0; cell_col < self->days; cell_col++)
        {
          startx = self->left_pad + cell_col * self->cell_width + self->pos_x;
          starty = self->header_height + cell_row * self->cell_height - 1 + self->pos_y;

          if (self->boxes[cell_row][cell_col])
            {
              self->boxes[cell_row][cell_col]->free_fn(self->boxes[cell_row][cell_col]);
            }
          self->boxes[cell_row][cell_col] = cell = (Box *) daybox_new();
          box_init(cell, self->cell_height + 1, self->cell_width + 1, starty, startx);
          box_set_borders(cell, 0, 0, 0, 0, 0, 0, 0, 0);
          box_set_bg_color((Box *) cell, C_CELL_BORDER);

          box_draw(cell);

          if (self->dayboxes[cell_row][cell_col])
            {
              daybox_free(self->dayboxes[cell_row][cell_col]);
            }
          self->dayboxes[cell_row][cell_col] = daycell = daybox_new();
          daybox_init(daycell, self->cell_height - 1, self->cell_width - 1, starty + 1, startx + 1);

          day_offset = cell_row * 7 + cell_col - day_of_week + self->week_offset * 7;
          tm_info = _get_date_by_offset(day_offset);
          data = database_get_data(self->db, 1900 + tm_info->tm_year, tm_info->tm_mon + 1, tm_info->tm_mday);
          if (!data)
            {
              data = daydata_new(1900 + tm_info->tm_year, tm_info->tm_mon + 1, tm_info->tm_mday);
              database_add_data(self->db, 1900 + tm_info->tm_year, tm_info->tm_mon + 1, tm_info->tm_mday, data);
            }
          daybox_set_data(daycell, data);

          if (day_offset == 0)
            {
              daybox_set_label_color(daycell, C_CELL_TODAY_LABEL);
              box_set_bg_color((Box *) daycell, C_CELL_SELECTED);
              self->selected_row = cell_row;
              self->selected_col = cell_col;
            }
          else if (daydata_is_holiday(daycell->data))
            {
              daybox_set_label_color(daycell, C_CELL_WEEKEND_LABEL);
              box_set_bg_color((Box *) daycell, C_CELL_WEEKEND);
            }
          else
            {
              daybox_set_label_color(daycell, C_CELL_WEEKDAY_LABEL);
              box_set_bg_color((Box *) daycell, C_CELL_WEEKDAY);
            }
        }
    }
}

int
calendar_init(Calendar *self, DataBase *database, int pos_x, int pos_y, int days, int weeks)
{
  int i;

  self->free_fn = _free;
  self->draw_fn = _draw;
  self->db = database;
  self->pos_x = pos_x;
  self->pos_y = pos_y;
  self->days = days;
  self->weeks = weeks;

  self->header_width = (COLS - pos_x) / self->days;
  self->header_height = 3;

  self->cell_width = (COLS - pos_x) / self->days;
  self->cell_height = ((LINES - pos_y) - self->header_height) / weeks;

  self->left_pad = ((COLS - self->days * self->cell_width) / 2);

  self->headers = calloc(1, sizeof(HeaderBox) * self->days);

  self->boxes = (Box ***) calloc(1, sizeof(Box *) * self->weeks);
  for (i = 0; i < self->weeks; i++)
    self->boxes[i] = (Box **) calloc(1, sizeof(Box) * self->days);

  self->dayboxes = (DayBox ***) calloc(1, sizeof(DayBox *) * self->weeks);
  for (i = 0; i < self->weeks; i++)
    self->dayboxes[i] = (DayBox **) calloc(1, sizeof(DayBox) * self->days);

  _calendar_init_headers(self);
  _calendar_init_cells(self);

  return 0;
}

Calendar *
calendar_new()
{
  Calendar *c = calloc(1, sizeof(Calendar));
  c->week_offset = 0;
  return c;
}

void
calendar_free(Calendar *self)
{
  self->free_fn(self);
}

void calendar_show_selected(Calendar *self)
{
  box_set_bg_color((Box *) self->dayboxes[self->selected_row][self->selected_col], C_CELL_SELECTED);
  box_draw((Box *) self->dayboxes[self->selected_row][self->selected_col]);
}

void calendar_hide_selected(Calendar *self)
{
  box_set_bg_color((Box *) self->dayboxes[self->selected_row][self->selected_col], C_CELL_WEEKDAY);
  box_draw((Box *) self->dayboxes[self->selected_row][self->selected_col]);
}

void calendar_set_selected(Calendar *self, int row, int col)
{
  calendar_hide_selected(self);
  self->selected_row = row;
  self->selected_col = col;
  calendar_show_selected(self);
}

void calendar_get_selected(Calendar *self, int *row, int *col)
{
  *row = self->selected_row;
  *col = self->selected_col;
}

DayData *calendar_get_selected_daydata(Calendar *self)
{
  return daybox_get_data(self->dayboxes[self->selected_row][self->selected_col]);
}

int calendar_get_week_offset(Calendar *self)
{
  return self->week_offset;
}

int
calendar_get_days_num(Calendar *self)
{
  return self->days;
}

int
calendar_get_weeks_num(Calendar *self)
{
  return self->weeks;
}

void calendar_set_week_offset(Calendar *self, int offset)
{
  if (offset == self->week_offset)
    return;

  self->week_offset = offset;
  _calendar_init_cells(self);
  calendar_draw(self);
}
