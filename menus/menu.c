#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "menu.h"

static void
_draw(Menu *self)
{
  wbkgd(self->win, COLOR_PAIR(self->bg_color));
  if (self->title)
    {
      wattron(self->win, COLOR_PAIR(self->txt_color));
      mvwaddwstr(self->win, 0, 0, self->title);
      wattroff(self->win, COLOR_PAIR(self->txt_color));
    }
  wrefresh(self->win);
}

static void
_free(Menu *self)
{
  delwin(self->win);
  //TODO endwin()?
  free(self);
}

void
menu_draw(Menu *self)
{
  self->draw_fn(self);
}

void
menu_use(Menu *self)
{
  if (self->use_fn)
    self->use_fn(self);
  return;
}

int
menu_init(Menu *self, int n_lines, int n_cols, int pos_y, int pos_x)
{
  if (pos_x < 0)
    pos_x = 0;
  if (pos_y < 0)
    pos_y = 0;

  if ((pos_x + n_cols) > COLS)
    n_cols = COLS - pos_x;
  if ((pos_y + n_lines) > LINES)
    n_lines = LINES - pos_y;

  self->win = newwin(n_lines, n_cols, pos_y, pos_x);
  if (!self->win)
    {
      return ERR;
    }

  self->free_fn = _free;
  self->draw_fn = _draw;
  self->use_fn = NULL;

  return 0;
}

Menu *
menu_new(void)
{
  Menu *m;
  m = malloc(sizeof(Menu));
  if (m)
    memset(m, 0, sizeof(Menu));
  return m;
}

void
menu_free(Menu *self)
{
  self->free_fn(self);
}

void
menu_set_bg_color(Menu *self, int pair_id)
{
  self->bg_color = pair_id;
}

void
menu_set_txt_color(Menu *self, int pair_id)
{
  self->txt_color = pair_id;
}

void
menu_set_title(Menu *self, wchar_t *title)
{
  self->title = title;
}

void
menu_get_pos(Menu *self, int *y, int *x)
{
  getbegyx(self->win, (*y), (*x));
}

void
menu_get_size(Menu *self, int *y, int *x)
{
  getmaxyx(self->win, (*y), (*x));
}

WINDOW *
menu_get_win(Menu *self)
{
  return self->win;
}
