#ifndef DAYDATA_H
#define DAYDATA_H

#include "settings.h"

typedef struct _DayData DayData;

DayData *daydata_new(int year, int month, int day);

void daydata_start(DayData *self);
int daydata_add(DayData *self, char *str, int color);
void daydata_del_list(DayData *self);
int daydata_get_next_data(DayData *self, char *str, int *color);
int daydata_get_day(DayData *self);
int daydata_get_month(DayData *self);
int daydata_get_year(DayData *self);
void daydata_free(DayData *self);
void daydata_set_holiday(DayData *self, int holiday);
int daydata_is_holiday(DayData *self);

#endif
