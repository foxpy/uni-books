#include "database-wrapper_impl/database-wrapper_impl.h"

static ptrdiff_t get_games_count(sqlite3* conn, bool deleted_games, char** err) {
    int rc;
    char* query = sprintf_alloc("SELECT count(game_id) FROM Games WHERE exists_flag = %s;",
                                (deleted_games) ? "FALSE" : "TRUE");
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(conn, query, STMT_NULL_TERMINATED, &stmt, NULL);
    free(query);
    if (rc) {
        *err = sprintf_alloc("Failed to get number of games: %s", sqlite3_errstr(rc));
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_step(stmt);
    sqlite3_uint64 ret = sqlite3_column_int64(stmt, 0);
    sqlite3_finalize(stmt);
    return ret;
}

ptrdiff_t database_get_games(db* database, bool deleted_games, struct game** dst, char** err) {
    sqlite3_stmt* stmt;
    {
        int rc;
        char* query = sprintf_alloc("SELECT name, author FROM Games WHERE exists_flag = %s;",
                                    (deleted_games) ? "FALSE" : "TRUE");
        rc = sqlite3_prepare_v2(database->db_file, query, STMT_NULL_TERMINATED, &stmt, NULL);
        free(query);
        if (rc) {
            *err = sprintf_alloc("Failed to get list of games: %s", sqlite3_errstr(rc));
            sqlite3_finalize(stmt);
            return -1;
        }
    }
    ptrdiff_t num_games = get_games_count(database->db_file, deleted_games, err);
    if (num_games < 0) {
        sqlite3_finalize(stmt);
        return -1;
    } else if (num_games == 0) {
        *dst = NULL;
        sqlite3_finalize(stmt);
        return 0;
    } else {
        struct game* games = emalloc(sizeof(struct game) * num_games);
        *dst = games;
        for (size_t i = 0, end = num_games; i < end; ++i) {
            sqlite3_step(stmt);
            games[i].name = sprintf_alloc("%s", sqlite3_column_text(stmt, 0));
            games[i].author = sprintf_alloc("%s", sqlite3_column_text(stmt, 1));
        }
        sqlite3_finalize(stmt);
        return num_games;
    }
}
