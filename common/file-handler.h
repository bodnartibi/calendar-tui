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
