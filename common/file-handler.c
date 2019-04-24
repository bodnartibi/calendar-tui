/*
 * Copyright (c) 2019 Tibor Bodnar <piratetibi@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 2 as published
 * by the Free Software Foundation, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lessert General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "file-handler.h"
#include "daydata.h"
#include "database.h"

struct _FileHandler
{
  DataBase *db;
  char path[MAX_PATH_LEN];
};

FileHandler *
filehandler_new(DataBase *db, char *path)
{
  FileHandler *f;

  if (!db || !path)
    return NULL;

  f = calloc(1, sizeof(FileHandler));
  f->db = db;
  strncpy(f->path, path, sizeof(f->path));

  return f;
}

//TODO macro
static int
_string_to_color(char *str)
{
  if (strcmp(VACATION, str) == 0)
    return C_VACATION;
  if (strcmp(NOT_WORKING, str) == 0)
    return C_NOT_WORKING;
  if (strcmp(CONFERENCE, str) == 0)
    return C_CONFERENCE;
  if (strcmp(HOME_OFFICE, str) == 0)
    return C_HOME_OFFICE;
  if (strcmp(SICK_LEAVE, str) == 0)
    return C_SICK_LEAVE;
  if (strcmp(LIFE_LONG_LEARNING, str) == 0)
    return C_LIFE_LONG_LEARNING;
  if (strcmp(TRAINING, str) == 0)
    return C_TRAINING;
  if (strcmp(NON_PAID_HOLIDAY, str) == 0)
    return C_NON_PAID_HOLIDAY;
  if (strcmp(HALF_HOLIDAY_AM, str) == 0)
    return C_HALF_HOLIDAY_AM;
  if (strcmp(BUSINESS_TRIP, str) == 0)
    return C_HALF_HOLIDAY_PM;
  if (strcmp(STANDBY, str) == 0)
    return C_BUSINESS_TRIP;
  return 0;
}

// Don't judge me, I know there is a better way to do this
int
filehandler_read_files(FileHandler *self)
{
  DIR *d;
  struct dirent *dir;
  char filepath[256 + MAX_PATH_LEN];
  char filename[256];
  char *pch;
  int year, month, day;
  int state;
  FILE *fp;
  char *line;
  char *newline_pos;
  size_t len = 0;

  DayData *dd;

  d = opendir(self->path);

  if (d)
    {
      while ((dir = readdir(d)) != NULL)
        {
          if (strcmp(".", dir->d_name) == 0 || strcmp("..", dir->d_name) == 0)
            continue;

          snprintf(filename, 256, "%s", dir->d_name);

          state = 0, year = 0, month = 0, day = 0;
          pch = strtok(filename, "._");
          while (pch != NULL)
            {
              switch (state)
                {
                  case 0:
                    year = atoi(pch);
                  state = 1;
                  break;
                  case 1:
                    month = atoi(pch);
                  state = 2;
                  break;
                  case 2:
                    day = atoi(pch);
                  state = 3;
                  break;
                  case 3:
                    // .txt
                    state = 0;
                  break;
                }
              pch = strtok(NULL, "._");
            }

          snprintf(filepath, sizeof(filename), "%s%s", self->path, dir->d_name);
          fp = fopen(filepath, "r");
          if (fp == NULL)
            return -1;

          line = NULL;
          len = 0;

          // FIXME
          if (year == 0)
            {
              while ((getline(&line, &len, fp)) != -1)
                {
                  if ((newline_pos = strchr(line, '\n')) != NULL)
                    *newline_pos = '\0';
                }
              database_add_alert(self->db, line);
            }
          else
            {
              dd = daydata_new(year, month, day);
              filehandler_parse_file(self, dd, fp);
              database_add_data(self->db, year, month, day, dd);
            }

          fclose(fp);
          free(line);
        }

      closedir(d);
    }

  return 0;
}

void
filehandler_parse_file(FileHandler *self, DayData *dd, FILE *fp)
{
  char *pch;
  int holiday;
  int state;
  char *line;
  char *newline_pos;
  size_t len = 0;
  ssize_t read;
  int color = C_DEFAULT;

  // first line is about vacation or not
  read = getline(&line, &len, fp);
  if (read == -1)
    return;

  holiday = !strcmp(HOLIDAY, line);
  daydata_set_holiday(dd, holiday);

  while ((getline(&line, &len, fp)) != -1)
    {
      if ((newline_pos = strchr(line, '\n')) != NULL)
        *newline_pos = '\0';

      state = 0;
      pch = strtok(line, ";");
      while (pch != NULL)
        {
          switch (state)
            {
              case 0:
                color = _string_to_color(pch);
              state++;
              break;
              case 1:
                daydata_add(dd, pch, color);
              state++;
              break;
            }
          pch = strtok(NULL, "._");
        }
    }
}

void
filehandler_free(FileHandler *self)
{
  DayData *dd;

  database_start(self->db);

  while (1)
    {
      dd = (DayData *) database_get_next_data(self->db);
      if (!dd)
        break;

      daydata_free(dd);
    }

  free(self);
}
