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
#ifndef DAYDATA_H
#define DAYDATA_H

#include "settings.h"

typedef struct _DayData DayData;

DayData *daydata_new(int year, int month, int day);

void daydata_start(DayData *self);
int daydata_add(DayData *self, char *str, int color);
void daydata_del_list(DayData *self);
int daydata_get_next_data(DayData *self, char *str, int *color);
int daydata_get_day(DayData *self);
int daydata_get_month(DayData *self);
int daydata_get_year(DayData *self);
void daydata_free(DayData *self);
void daydata_set_holiday(DayData *self, int holiday);
int daydata_is_holiday(DayData *self);

#endif
