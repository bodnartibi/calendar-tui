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
