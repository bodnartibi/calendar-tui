#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "settings.h"
#include "box.h"
#include "menu-box.h"

typedef struct _Option
{
  wchar_t *name;
  MENU_BOX_OPTION_CB cb;
  struct _Option *next;
} Option;

struct _MenuBox
{
  Box super;
  Option *options;
  int selected;
  int options_num;
  int selected_color;
  int width;
};

static void _draw(Box *super)
{
  MenuBox *self = (MenuBox *) super;
  Option *opt;
  int i = 0;

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

  for (opt = self->options; opt != NULL; opt = opt->next)
    {
      wmove(self->super.win, i + 1, 1);
      if (i == self->selected)
        wattron(self->super.win, COLOR_PAIR(self->selected_color) | A_BOLD);

      waddwstr(self->super.win, opt->name);

      if (i == self->selected)
        wattroff(self->super.win, COLOR_PAIR(self->selected_color) | A_BOLD);
      i++;
    }

  wrefresh(self->super.win);
}

static void _free(Box *box)
{
  MenuBox *self = (MenuBox *) box;

  Option *opt = self->options;
  while (opt)
    {
      self->options = self->options->next;
      free(opt);
      opt = self->options;
    }
  //TODO endwin()?
  delwin(self->super.win);
  free(self);
}

void
menubox_call(MenuBox *self)
{
  int i = 0;
  Option *opt = self->options;

  if (!opt)
    return;

  for (; opt != NULL && i != self->selected; i++, opt = opt->next);

  if (opt->cb)
    opt->cb();
}

void
menubox_move_up(MenuBox *self)
{
  if (self->selected == 0)
    return;
  self->selected--;
}

void
menubox_move_down(MenuBox *self)
{
  if (self->selected < self->options_num - 1)
    self->selected++;
}

int
menubox_add_option(MenuBox *self, wchar_t *name, MENU_BOX_OPTION_CB cb)
{
  Option *o;
  int len;

  if (!self->options)
    {
      self->options = calloc(1, sizeof(Option));
      if (!self->options)
        return ERR;
      o = self->options;
    }
  else
    {
      for (o = self->options; o->next != NULL; o = o->next);

      o->next = calloc(1, sizeof(Option));
      o = o->next;
    }

  if (!o)
    return ERR;

  self->options_num++;
  o->name = name;
  o->cb = cb;

  len = wcslen(name);
  if (len > self->width)
    self->width = len;

  wresize(((Box *) self)->win, self->options_num + 2, self->width + 2);

  return 0;
}

void
menubox_set_selected_color(MenuBox *self, int selected_color)
{
  self->selected_color = selected_color;
}

void
menubox_set_selected(MenuBox *self, int selected)
{
  self->selected = selected;
}

int
menubox_init(MenuBox *self, int pos_y, int pos_x)
{
  if (box_init((Box *) self, 1, 0, pos_y, pos_x) != 0)
    {
      return ERR;
    }

  self->super.free_fn = _free;
  self->super.draw_fn = _draw;
  self->options = NULL;
  self->selected = 0;
  self->options_num = 0;
  self->width = 0;
  self->selected_color = C_DEFAULT;
  return 0;
}

MenuBox *
menubox_new(void)
{
  MenuBox *h;
  h = malloc(sizeof(MenuBox));
  if (h)
    memset(h, 0, sizeof(MenuBox));
  return h;
}
