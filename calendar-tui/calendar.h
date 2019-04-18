#ifndef CALENDAR_H
#define CALENDAR_H

#include "settings.h"
#include "box.h"
#include "header-box.h"
#include "day-box.h"
#include "database.h"

typedef struct _Calendar Calendar;

struct _Calendar
{
  void (*free_fn)(Calendar *self);
  void (*draw_fn)(Calendar *self);

  DataBase *db;
  HeaderBox **headers;
  Box ***boxes;
  DayBox ***dayboxes;

  int week_offset;
  int selected_row, selected_col;
  int pos_x, pos_y;
  int days, weeks;
  int cell_width, cell_height;
  int left_pad;
  int header_width, header_height;
};

Calendar *calendar_new();
void calendar_free(Calendar *self);
void calendar_draw(Calendar *self);
int calendar_init(Calendar *self, DataBase *database, int pos_x, int pos_y, int days, int weeks);

void calendar_get_selected(Calendar *self, int *row, int *col);
void calendar_set_selected(Calendar *self, int row, int col);
void calendar_hide_selected(Calendar *self);

int calendar_get_days_num(Calendar *self);
int calendar_get_weeks_num(Calendar *self);

int calendar_get_week_offset(Calendar *self);
void calendar_set_week_offset(Calendar *self, int offset);

DayData *calendar_get_selected_daydata(Calendar *self);

#endif
