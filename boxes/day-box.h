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
#ifndef DAY_H
#define DAY_H

#include "daydata.h"

typedef struct _DayBox DayBox;

struct _DayBox
{
  Box super;
  DayData *data;
  chtype label_color;
};

DayBox *daybox_new();
int daybox_init(DayBox *self, int n_lines, int n_cols, int pos_y, int pos_x);
int daybox_set_data(DayBox *self, DayData *data);
DayData *daybox_get_data(DayBox *self);
int daybox_set_label_color(DayBox *self, chtype color);
void daybox_free(DayBox *self);

#endif
