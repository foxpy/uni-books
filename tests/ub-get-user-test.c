#include <qc.h>
#include <string.h>
#include <sqlite3.h>
#include "database-wrapper_impl/database-wrapper_impl.h"

static char const* username = "admin";
static char const* password_hash = "Some long n scary hash string";

static void add_test_user(sqlite3* db) {
    static char const* query = "INSERT INTO Users(username, password_hash) VALUES(?1, ?2);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, query, STMT_NULL_TERMINATED, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, username, STMT_NULL_TERMINATED, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password_hash, STMT_NULL_TERMINATED, SQLITE_STATIC);
    int rc = sqlite3_step(stmt);
    qc_assert(rc == SQLITE_DONE, sqlite3_errstr(rc));
    sqlite3_finalize(stmt);
}

int main() {
    struct user user = {0};
    sqlite3* conn;
    int rc;
    char* err;
    bool result;
    rc = sqlite3_open_v2("test", &conn, SQLITE_OPEN_MEMORY | SQLITE_OPEN_READWRITE, NULL);
    qc_assert(rc == SQLITE_OK, sqlite3_errstr(rc));
    rc = sqlite3_exec(conn, schema, NULL, NULL, &err);
    qc_assert(rc == SQLITE_OK, err);
    add_test_user(conn);
    qc_err* qcerr = qc_err_new();
    result = get_user("admin", conn, &user, qcerr);
    qc_assert_format(result == QC_SUCCESS, "Failed to get user which sure exists in a database: %s", qc_err_get(qcerr));
    qc_assert(strcmp(user.username, username) == 0, "Username does not match");
    qc_assert(strcmp(user.password_hash, password_hash) == 0, "Password hash does not match");
    free(user.username);
    free(user.password_hash);
    result = get_user("manager", conn, &user, qcerr);
    qc_assert_format(result == QC_SUCCESS, "get_user subroutine has failed: %s", qc_err_get(qcerr));
    qc_assert(user.username == NULL, "Got username for user which sure does not exist");
    qc_assert(user.password_hash == NULL, "Got password hash for user which sure does not exist");
    sqlite3_close(conn);
    qc_err_free(qcerr);
}
