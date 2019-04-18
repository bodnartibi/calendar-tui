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
