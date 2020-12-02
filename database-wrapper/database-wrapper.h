#pragma once
#include <sqlite3.h>
#include <stdbool.h>

typedef enum {
    DATABASE_USER,
    DATABASE_ADMIN,
    DATABASE_UNAUTHORIZED,
} db_user_type;

typedef struct database {
    sqlite3* db_file;
} db;

bool database_new(char const* path, char** err, char const* admin_password);
db* database_open(char const* path, char** err);
void database_close(db* database);

bool database_register(db* database, char const* username, char const* password, bool is_admin, char** err);
db_user_type database_check_credentials(db* database, char const* username, char const* password, char** err);
