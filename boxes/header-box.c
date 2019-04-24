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
#include <wchar.h>

#include "settings.h"
#include "box.h"
#include "header-box.h"

static void _draw(Box *super)
{
  HeaderBox *self = (HeaderBox *) super;
  int width = box_get_width(super);

  wborder(self->super.win, \
          self->super.borders[0], \
          self->super.borders[1], \
          self->super.borders[2], \
          self->super.borders[3], \
          self->super.borders[4], \
          self->super.borders[5], \
          self->super.borders[6], \
          self->super.borders[7]);
  wbkgd(self->super.win, COLOR_PAIR(self->super.pair_id));

  if (self->title)
    {
      wattron(self->super.win, COLOR_PAIR(self->title_color) | A_BOLD);
      mvwaddwstr(self->super.win, 1, width / 2 - wcslen(self->title) / 2, self->title);
      wattroff(self->super.win, COLOR_PAIR(self->title_color) | A_BOLD);
    }

  wrefresh(self->super.win);
}

static void _free(Box *box)
{
  HeaderBox *self = (HeaderBox *) box;
  //TODO endwin()?
  delwin(self->super.win);
  free(self);
}

void
headerbox_set_title_color(HeaderBox *self, int color)
{
  self->title_color = color;
}

int
headerbox_init(HeaderBox *self, int n_lines, int n_cols, int pos_y, int pos_x)
{
  if (box_init((Box *) self, n_lines, n_cols, pos_y, pos_x) != 0)
    {
      return ERR;
    }

  self->title_color = C_DEFAULT;
  self->super.free_fn = _free;
  self->super.draw_fn = _draw;
  return 0;
}

HeaderBox *
headerbox_new(void)
{
  return (HeaderBox *) calloc(1, sizeof(HeaderBox));
}

int
headerbox_set_title(HeaderBox *self, wchar_t *title)
{
  self->title = title;
  return 0;
}
