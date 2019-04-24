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
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "settings.h"
#include "box.h"
#include "day-box.h"
#include "daydata.h"

size_t
_utf8_strlen(const char *s)
{
  size_t count = 0;
  while (*s)
    {
      count += (*s++ & 0xC0) != 0x80;
    }
  return count;
}

static void
_draw(Box *super)
{
  DayBox *self = (DayBox *) super;
  char *str;
  size_t str_length;
  int color;
  char truncated_format[10];
  int max_available, remaining;
  int width = box_get_width(super);
  int height = box_get_height(super);
  int row = 0;
  int ret;

  str = calloc(1, MAX_STR_LEN);

  werase(self->super.win);
  wbkgd(self->super.win, COLOR_PAIR(self->super.pair_id));

  if (self->data)
    {
      wattron(self->super.win, COLOR_PAIR(self->label_color) | A_BOLD);
      mvwaddch(self->super.win, 0, width - 3, ACS_VLINE);
      wprintw(self->super.win, "%2u", daydata_get_day(self->data));
      mvwaddch(self->super.win, 1, width - 3, ACS_LLCORNER);
      waddch(self->super.win, ACS_HLINE);
      waddch(self->super.win, ACS_HLINE);
      wattroff(self->super.win, COLOR_PAIR(self->label_color) | A_BOLD);

      daydata_start(self->data);
      max_available = remaining = width - 3;
      wmove(self->super.win, 0, 0);
      sprintf(truncated_format, "%%.%ds", max_available);

      while (1)
        {
          ret = daydata_get_next_data(self->data, str, &color);
          if (ret != 0)
            {
              wattroff(self->super.win, COLOR_PAIR(color) | A_BOLD);
              break;
            }
          str_length = _utf8_strlen(str);

          wattron(self->super.win, COLOR_PAIR(color) | A_BOLD);

          if (remaining == max_available)
            {
              wprintw(self->super.win, truncated_format, str);
              remaining -= str_length;
            }
          else
            {
              if (remaining < str_length + 2)
                {
                  row++;
                  if (row >= height)
                    {
                      wattroff(self->super.win, COLOR_PAIR(color) | A_BOLD);
                      break;
                    }
                  wmove(self->super.win, row, 0);
                  max_available = remaining = (row > 1) ? width : (width - 3);
                  sprintf(truncated_format, "%%.%ds", max_available);
                  wprintw(self->super.win, truncated_format, str);
                  remaining -= str_length;
                }
              else
                {
                  sprintf(truncated_format, ", %%.%ds", remaining);
                  wprintw(self->super.win, truncated_format, str);
                  remaining -= str_length + 2;
                }
            }
        }
    }

  wrefresh(self->super.win);
  free(str);
}

static void
_free(Box *box)
{
  DayBox *self = (DayBox *) box;
  //TODO endwin()?
  delwin(self->super.win);
  free(self);
}

int
daybox_init(DayBox *self, int n_lines, int n_cols, int pos_y, int pos_x)
{
  // at the right borders we need some magic
  // we need space to print the box border
  if ((pos_x + n_cols) >= COLS)
    n_cols = COLS - pos_x - 1;
  if ((pos_y + n_lines) >= LINES)
    n_lines = LINES - pos_y - 1;

  if (box_init((Box *) self, n_lines, n_cols, pos_y, pos_x) != 0)
    {
      return ERR;
    }

  self->super.free_fn = _free;
  self->super.draw_fn = _draw;
  return 0;
}

DayBox *
daybox_new(int pos_y, int pos_x, int n_lines, int n_cols)
{
  return calloc(1, sizeof(DayBox));
}

int
daybox_set_data(DayBox *self, DayData *data)
{
  self->data = data;
  return 0;
}

int
daybox_set_label_color(DayBox *self, chtype color)
{
  self->label_color = color;
  return 0;
}

DayData *
daybox_get_data(DayBox *self)
{
  return self->data;
}

void
daybox_free(DayBox *self)
{
  free(self);
}
