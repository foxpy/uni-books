#include <stdbool.h>
#include <string.h>
#include <sqlite3.h>
#include <assert.h>
#include <qc.h>
#include "database-wrapper_impl.h"

qc_result get_user(char const* username, sqlite3* db, struct user* user, qc_err* err) {
    if (!validate_username(username, err)) {
        return QC_FAILURE;
    } else {
        int rc;
        sqlite3_stmt* stmt;
        char const* query = "SELECT username, password_hash FROM Users WHERE username LIKE ?1";
        if ((rc = sqlite3_prepare_v2(db, query, STMT_NULL_TERMINATED, &stmt, NULL)) != SQLITE_OK ||
            (rc = sqlite3_bind_text(stmt, 1, username, STMT_NULL_TERMINATED, SQLITE_TRANSIENT)) != SQLITE_OK) {
            qc_err_set(err, "Failed to get list of users: %s", sqlite3_errstr(rc));
            sqlite3_finalize(stmt);
            return QC_FAILURE;
        }
        unsigned char const* text;
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_ROW) {
            user->username = NULL;
            user->hash = NULL;
            sqlite3_finalize(stmt);
            return QC_SUCCESS;
        }
        assert(rc == SQLITE_ROW);
        text = sqlite3_column_text(stmt, 0);
        assert(sqlite3_column_bytes(stmt, 0) > 0);
        user->username = emalloc(strlen((char const*) text));
        strcpy(user->username, (char const*) text);
        text = sqlite3_column_text(stmt, 1);
        assert(sqlite3_column_bytes(stmt, 1) > 0);
        user->hash = emalloc(strlen((char const*) text));
        strcpy(user->hash, (char const*) text);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            qc_err_set(err, "Failed to get user %s: %s", username, sqlite3_errstr(rc));
            sqlite3_finalize(stmt);
            return QC_FAILURE;
        }
        sqlite3_finalize(stmt);
        return QC_SUCCESS;
    }
}
