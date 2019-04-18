#ifndef HEADER_H
#define HEADER_H

typedef struct _HeaderBox HeaderBox;

struct _HeaderBox
{
  Box super;
  wchar_t *title;
  int title_color;
};

HeaderBox *headerbox_new(void);
int headerbox_init(HeaderBox *self, int n_lines, int n_cols, int pos_y, int pos_x);
void headerbox_set_title_color(HeaderBox *self, int color);
int headerbox_set_title(HeaderBox *self, wchar_t *title);

#endif
