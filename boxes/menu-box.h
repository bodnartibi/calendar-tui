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
#ifndef MENU_BOX_H
#define MENU_BOX_H

typedef struct _MenuBox MenuBox;
typedef void (*MENU_BOX_OPTION_CB)(void);

MenuBox *menubox_new(void);
int menubox_init(MenuBox *self, int pos_y, int pos_xm);

void menubox_call(MenuBox *self);
void menubox_move_up(MenuBox *self);
void menubox_move_down(MenuBox *self);
int menubox_add_option(MenuBox *self, wchar_t *name, MENU_BOX_OPTION_CB cb);
void menubox_set_selected_color(MenuBox *self, int selected_color);
void menubox_set_selected(MenuBox *self, int selected);

#endif
