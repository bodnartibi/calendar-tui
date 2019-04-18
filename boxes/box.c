#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "box.h"

static void _draw(Box *self)
{
  wborder(self->win, \
            self->borders[0], \
            self->borders[1], \
            self->borders[2], \
            self->borders[3], \
            self->borders[4], \
            self->borders[5], \
            self->borders[6], \
            self->borders[7]);
  wbkgd(self->win, COLOR_PAIR(self->pair_id));
  wrefresh(self->win);
}

static void _free(Box *self)
{
  delwin(self->win);
  //TODO endwin()?
  free(self);
}

void
box_draw(Box *self)
{
  self->draw_fn(self);
}

int
box_init(Box *self, int n_lines, int n_cols, int pos_y, int pos_x)
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

  return 0;
}

Box *
box_new()
{
  return (Box *) calloc(1, sizeof(Box));
}

void
box_free(Box *self)
{
  self->free_fn(self);
}

int
box_set_win_params(Box *self, int n_lines, int n_cols, int pos_y, int pos_x)
{
  int ret = 0;

  ret = mvwin(self->win, pos_y, pos_x);
  if (ret == ERR)
    return ret;

  ret = wresize(self->win, n_lines, n_cols);
  if (ret == ERR)
    return ret;

  return 0;
}

void
box_set_bg_color(Box *self, int pair_id)
{
  self->pair_id = pair_id;
}

int
box_set_borders(Box *self, chtype ls, chtype rs, chtype ts, chtype bs, chtype tl, chtype tr, chtype bl, chtype br)
{
  self->borders[0] = ls;
  self->borders[1] = rs;
  self->borders[2] = ts;
  self->borders[3] = bs;
  self->borders[4] = tl;
  self->borders[5] = tr;
  self->borders[6] = bl;
  self->borders[7] = br;
  return 0;
}

void
box_get_sizes(Box *self, int *y, int *x)
{
  getmaxyx(self->win, (*y), (*x));
}

int
box_get_width(Box *self)
{
  return getmaxx(self->win);
}

int
box_get_height(Box *self)
{
  return getmaxy(self->win);
}

void
box_get_pos(Box *self, int *y, int *x)
{
  getbegyx(self->win, (*y), (*x));
}

WINDOW *
box_get_win(Box *self)
{
  return self->win;
}
