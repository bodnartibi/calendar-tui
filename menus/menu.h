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
#ifndef MENU_H
#define MENU_H

typedef struct _Menu Menu;

struct _Menu
{
  void (*free_fn)(Menu *self);
  void (*draw_fn)(Menu *self);
  void (*use_fn)(Menu *self);

  WINDOW *win;

  wchar_t *title;
  int bg_color;
  int txt_color;
};

Menu *menu_new(void);
void menu_free(Menu *self);
void menu_draw(Menu *self);
void menu_use(Menu *self);
int menu_init(Menu *self, int n_lines, int n_cols, int pos_y, int pos_x);

void menu_set_bg_color(Menu *self, int pair_id);
void menu_set_txt_color(Menu *self, int pair_id);
void menu_set_title(Menu *self, wchar_t *title);
void menu_get_size(Menu *self, int *y, int *x);
void menu_get_pos(Menu *self, int *y, int *x);

WINDOW *menu_get_win(Menu *self);

#endif //MENU_H
