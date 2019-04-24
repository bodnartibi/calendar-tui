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
#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "settings.h"
#include "database.h"
#include "daydata.h"

typedef struct _FileHandler FileHandler;

FileHandler *filehandler_new(DataBase *db, char *path);
int filehandler_read_files(FileHandler *self);
void filehandler_parse_file(FileHandler *self, DayData *dd, FILE *fp);
void filehandler_free(FileHandler *self);

#endif
