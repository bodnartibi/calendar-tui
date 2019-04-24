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
#ifndef CALENDAR_H
#define CALENDAR_H

#include "settings.h"
#include "box.h"
#include "header-box.h"
#include "day-box.h"
#include "database.h"

typedef struct _Calendar Calendar;

struct _Calendar
{
  void (*free_fn)(Calendar *self);
  void (*draw_fn)(Calendar *self);

  DataBase *db;
  HeaderBox **headers;
  Box ***boxes;
  DayBox ***dayboxes;

  int week_offset;
  int selected_row, selected_col;
  int pos_x, pos_y;
  int days, weeks;
  int cell_width, cell_height;
  int left_pad;
  int header_width, header_height;
};

Calendar *calendar_new();
void calendar_free(Calendar *self);
void calendar_draw(Calendar *self);
int calendar_init(Calendar *self, DataBase *database, int pos_x, int pos_y, int days, int weeks);

void calendar_get_selected(Calendar *self, int *row, int *col);
void calendar_set_selected(Calendar *self, int row, int col);
void calendar_hide_selected(Calendar *self);

int calendar_get_days_num(Calendar *self);
int calendar_get_weeks_num(Calendar *self);

int calendar_get_week_offset(Calendar *self);
void calendar_set_week_offset(Calendar *self, int offset);

DayData *calendar_get_selected_daydata(Calendar *self);

#endif
