#include "database-wrapper_impl/database-wrapper_impl.h"

bool database_add_book(db* database, char const* bookname, char const* author, char** err) {
    UNUSED(err);
    char const* query = "INSERT INTO Books(name, author) VALUES(?1, ?2);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(database->db_file, query, STMT_NULL_TERMINATED, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, bookname, STMT_NULL_TERMINATED, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, author, STMT_NULL_TERMINATED, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return true;
}
