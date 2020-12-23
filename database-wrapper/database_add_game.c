#include "database-wrapper_impl/database-wrapper_impl.h"

bool database_add_game(db* database, char const* gamename, char const* publisher, char const* score, char** err) {
    UNUSED(err);
    char const* query = "INSERT INTO Games(name, exists_flag, added_date, publisher, score) "
                        "VALUES(?1, TRUE, datetime(), ?2, ?3);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(database->db_file, query, STMT_NULL_TERMINATED, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, gamename, STMT_NULL_TERMINATED, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, publisher, STMT_NULL_TERMINATED, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, score, STMT_NULL_TERMINATED, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return true;
}
