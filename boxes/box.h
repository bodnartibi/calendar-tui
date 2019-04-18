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
