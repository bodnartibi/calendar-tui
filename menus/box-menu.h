#ifndef RANGE_MENU_H
#define RANGE_MENU_H

#include "menu-box.h"

typedef struct _BoxMenu BoxMenu;

BoxMenu *boxmenu_new(void);
int boxmenu_init(BoxMenu *self, MenuBox *box, int n_lines, int n_cols, int pos_y, int pos_x, wchar_t *title);
MenuBox *boxmenu_get_menubox(BoxMenu *self);

#endif //RANGE_MENU
