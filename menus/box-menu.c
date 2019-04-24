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

#include "menu.h"
#include "box-menu.h"
#include "box.h"
#include "menu-box.h"
#include "settings.h"

struct _BoxMenu
{
  Menu super;
  MenuBox *box;
};

void
_free(Menu *super)
{
  BoxMenu *self = (BoxMenu *) super;
  delwin(super->win);
  box_free((Box *) self->box);
  free(self);
}

void
_use(Menu *super)
{
  int run = 1;
  BoxMenu *self = (BoxMenu *) super;
  box_draw((Box *) self->box);

  // TODO
  // HUP signal handling?
  // should move out of this class?
  while (run)
    {
      switch (getch())
        {
          case KEY_UP:
            menubox_move_up(self->box);
          break;
          case KEY_DOWN:
            menubox_move_down(self->box);
          break;
          case 10:
            menubox_call(self->box);
          run = 0;
          break;
          case 'q':
            run = 0;
          break;
          default:
            refresh();
          break;
        }
      box_draw((Box *) self->box);
    }

}

MenuBox *
boxmenu_get_menubox(BoxMenu *self)
{
  return self->box;
}

int
boxmenu_init(BoxMenu *self, MenuBox *box, int n_lines, int n_cols, int pos_y, int pos_x, wchar_t *title)
{
  if (!title)
    return ERR;
  //TODO leftpad
  menu_init((Menu *) self, n_lines, n_cols, pos_y, pos_x);
  menu_set_title((Menu *) self, title);
  self->super.use_fn = _use;
  self->super.free_fn = _free;
  self->box = box;

  return 0;
}

BoxMenu *
boxmenu_new(void)
{
  BoxMenu *m;
  m = malloc(sizeof(BoxMenu));
  if (m)
    memset(m, 0, sizeof(BoxMenu));
  return m;
}
