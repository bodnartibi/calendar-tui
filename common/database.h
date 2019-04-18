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
