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
#ifndef BOX_H
#define BOX_H

typedef struct _Box Box;

struct _Box
{
  void (*free_fn)(Box *self);
  void (*draw_fn)(Box *self); // Draw the whole box, everything included

  WINDOW *win;

  int pair_id;
  chtype borders[8];
};

Box *box_new();
void box_free(Box *self);
void box_draw(Box *self);
int box_init(Box *self, int n_lines, int n_cols, int pos_y, int pos_x);

int box_set_win_params(Box *self, int n_lines, int n_cols, int pos_y, int pos_x);
void box_set_bg_color(Box *self, int pair_id);
int box_set_borders(Box *self, chtype ls, chtype rs, chtype ts, chtype bs, chtype tl, chtype tr, chtype bl, chtype br);

void box_get_pos(Box *self, int *y, int *x);
void box_get_sizes(Box *self, int *y, int *x);
int box_get_width(Box *self);
int box_get_height(Box *self);

WINDOW *box_get_win(Box *self);

#endif
