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
#ifndef DATABASE_H
#define DATABASE_H

typedef struct _DataBase DataBase;

DataBase *database_new();
void *database_get_data(DataBase *self, int year, unsigned int month, unsigned int day);
int database_add_data(DataBase *self, int year, unsigned int month, unsigned int day, void *data);
void database_start(DataBase *self);
void *database_get_next_data(DataBase *self);
void database_add_alert(DataBase *self, char *alert_message);
char *database_get_alert(DataBase *self);
void database_free(DataBase *self);

#endif
