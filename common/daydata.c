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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "daydata.h"
#include "text.h"

struct _DayData
{
  int day;
  int month;
  int year;
  int holiday;
  Text *list;
  Text *list_pos;
};

int static
_add_to_list(DayData *self, wchar_t *e, int color)
{
  Text *t;

  if (!self->list)
    {
      self->list = calloc(1, sizeof(Text));
      self->list->str = e;
      self->list->color = color;
      return 0;
    }

  for (t = self->list; t->next; t = t->next);

  t->next = calloc(1, sizeof(Text));
  t->next->str = e;
  t->next->color = color;
  return 0;
}

void static
_text_free(Text *t)
{
  if (t && t->str)
    free(t->str);
  if (t)
    free(t);
}

DayData *
daydata_new(int year, int month, int day)
{
  DayData *d = calloc(1, sizeof(DayData));

  d->day = day;
  d->month = month;
  d->year = year;

  return d;
}

void
daydata_set_holiday(DayData *self, int holiday)
{
  self->holiday = holiday;
}

int
daydata_is_holiday(DayData *self)
{
  return self->holiday;
}


void
daydata_start(DayData *self)
{
  self->list_pos = self->list;
}

int
daydata_add(DayData *self, char *str, int color)
{
  wchar_t *elem;

  if (!str)
    return -1;

  elem = malloc(MAX_STR_LEN * sizeof(wchar_t));
  mbstowcs(elem, str, MAX_STR_LEN);

  if (!_add_to_list(self, elem, color))
    return -1;

  return 0;
}

int
daydata_get_next_data(DayData *self, char *str, int *color)
{
  if (self->list_pos)
    {
      wcstombs(str, self->list_pos->str, MAX_STR_LEN);
      *color = self->list_pos->color;
      self->list_pos = self->list_pos->next;
    }
  else
    return -1;

  return 0;
}

int
daydata_get_day(DayData *self)
{
  return self->day;
}

int
daydata_get_month(DayData *self)
{
  return self->month;
}

int
daydata_get_year(DayData *self)
{
  return self->year;
}

void
daydata_del_list(DayData *self)
{
  Text *p;

  p = self->list;
  while (p)
    {
      self->list = self->list->next;
      _text_free(p);
      p = self->list;
    }
}

void
daydata_free(DayData *self)
{
  daydata_del_list(self);
  free(self);
}
