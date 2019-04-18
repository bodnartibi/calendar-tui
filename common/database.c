#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "database.h"
#include "daydata.h"

typedef struct _Year Year;

struct _Year
{
  int year;
  void *days[12][31];
  Year *next;
};

struct _DataBase
{
  char *alert;
  Year *years;
  Year *years_pos;
  int month_pos;
  int day_pos;
};

DataBase *
database_new()
{
  return (DataBase *) calloc(1, sizeof(DataBase));
}

static Year *
_database_add_year(DataBase *self, int year)
{
  Year *y;

  if (!self->years)
    {
      y = calloc(1, sizeof(Year));
      y->year = year;
      self->years = y;
      return y;
    }

  for (y = self->years; y; y = y->next)
    {
      if (y->year == year)
        {
          return y;
        }
    }

  for (y = self->years; y->next; y = y->next);

  y->next = calloc(1, sizeof(Year));
  y = y->next;
  y->year = year;

  return y;
}

void *
database_get_data(DataBase *self, int year, unsigned int month, unsigned int day)
{
  Year *y;

  month--;
  day--;

  if (month > 11 || month < 0 || \
      day > 30 || day < 0)
    return NULL;

  for (y = self->years; y; y = y->next)
    {
      if (y->year == year)
        break;
    }

  if (!y)
    return NULL;

  return y->days[month][day];
}

int
database_add_data(DataBase *self, int year, unsigned int month, unsigned int day, void *data)
{
  Year *y;

  month--;
  day--;

  if (month > 11 || month < 0 || \
      day > 30 || day < 0)
    return -1;

  y = _database_add_year(self, year);
  if (!y)
    return -1;

  if (y->days[month][day])
    {
      daydata_free(y->days[month][day]);
    }
  y->days[month][day] = data;

  return 0;
}

static void
_database_find_next_data(DataBase *self)
{
  do
    {
      self->day_pos++;
      if (self->day_pos > 30)
        {
          self->day_pos = 0;
          self->month_pos++;
          if (self->month_pos > 11)
            {
              self->month_pos = 0;
              self->years_pos = self->years_pos->next;
            }
        }
    }
  while (self->years_pos && !self->years_pos->days[self->month_pos][self->day_pos]);
}

void
database_start(DataBase *self)
{
  self->years_pos = self->years;
  self->day_pos = -1;
  self->month_pos = 0;

  _database_find_next_data(self);
}

void *
database_get_next_data(DataBase *self)
{
  void *data;

  if (!self->years_pos)
    return NULL;

  data = self->years_pos->days[self->month_pos][self->day_pos];
  _database_find_next_data(self);

  return data;
}

void
database_add_alert(DataBase *self, char *alert_message)
{
  self->alert = malloc(strlen(alert_message) + 1);
  strcpy(self->alert, alert_message);
}

char *
database_get_alert(DataBase *self)
{
  return self->alert;
}

void
database_free(DataBase *self)
{
  Year *y, *y2;

  y = self->years;

  while (y)
    {
      y2 = y;
      y = y->next;
      free(y2);
    }

  free(self->alert);
  free(self);
}
