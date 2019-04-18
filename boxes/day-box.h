#ifndef DAY_H
#define DAY_H

#include "daydata.h"

typedef struct _DayBox DayBox;

struct _DayBox
{
  Box super;
  DayData *data;
  chtype label_color;
};

DayBox *daybox_new();
int daybox_init(DayBox *self, int n_lines, int n_cols, int pos_y, int pos_x);
int daybox_set_data(DayBox *self, DayData *data);
DayData *daybox_get_data(DayBox *self);
int daybox_set_label_color(DayBox *self, chtype color);
void daybox_free(DayBox *self);

#endif
