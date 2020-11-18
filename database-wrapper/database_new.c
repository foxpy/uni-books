#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <qc/error.h>
#include "database-wrapper.h"

static qc_result create_file_if_not_exists(char const* path, qc_err* err) {
    errno = 0;
    FILE* f = fopen(path, "w+");
    if (f == NULL) {
        qc_err_set(err, "Failed to create file %s: %s", path, strerror(errno));
        return QC_FAILURE;
    } else {
        fclose(f);
        return QC_SUCCESS;
    }
}

bool database_new(char const* path, char** err) {
    qc_err* qcerr = qc_err_new();
    if (create_file_if_not_exists(path, qcerr) == QC_FAILURE) {
        qc_err_append_front(qcerr, "Failed to create database");
        *err = qc_err_to_owned_c_str(qcerr);
        return false;
    } else {
        sqlite3* db;
        if (sqlite3_open(path, &db) != SQLITE_OK) {
            qc_err_set(qcerr, "Failed to create database %s: %s", path, sqlite3_errmsg(db));
            if (db != NULL) {
                sqlite3_close(db);
            }
            return false;
        }
        char* sqlite_error;
        if (sqlite3_exec(db, schema, NULL, NULL, &sqlite_error) != SQLITE_OK) {
            qc_err_set(qcerr, "Failed to create schema: %s", sqlite_error);
            sqlite3_free(sqlite_error);
            *err = qc_err_to_owned_c_str(qcerr);
            return false;
        }
        sqlite3_close(db);
        qc_err_free(qcerr);
        return true;
    }
}
