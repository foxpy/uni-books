#pragma once
#include <sqlite3.h>

typedef struct database {
    sqlite3* db_file;
} db;

db* database_open(char const* path, char** err);
void database_close(db* database);
