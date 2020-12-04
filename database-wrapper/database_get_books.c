#include "database-wrapper_impl/database-wrapper_impl.h"

static ptrdiff_t get_books_count(sqlite3* conn, char** err) {
    int rc;
    char const* query = "SELECT count(book_id) FROM Books;";
    sqlite3_stmt* stmt;
    if ((rc = sqlite3_prepare_v2(conn, query, STMT_NULL_TERMINATED, &stmt, NULL))) {
        *err = sprintf_alloc("Failed to get number of books: %s", sqlite3_errstr(rc));
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_step(stmt);
    sqlite3_uint64 ret = sqlite3_column_int64(stmt, 0);
    sqlite3_finalize(stmt);
    return ret;
}

ptrdiff_t database_get_books(db* database, struct book** dst, char** err) {
    sqlite3_stmt* stmt;
    {
        int rc;
        char const* query = "SELECT name, author FROM Books;";
        if ((rc = sqlite3_prepare_v2(database->db_file, query, STMT_NULL_TERMINATED, &stmt, NULL))) {
            *err = sprintf_alloc("Failed to get list of users: %s", sqlite3_errstr(rc));
            sqlite3_finalize(stmt);
            return -1;
        }
    }
    ptrdiff_t num_books = get_books_count(database->db_file, err);
    if (num_books < 0) {
        sqlite3_finalize(stmt);
        return -1;
    } else if (num_books == 0) {
        *dst = NULL;
        sqlite3_finalize(stmt);
        return 0;
    } else {
        struct book* books = emalloc(sizeof(struct book) * num_books);
        *dst = books;
        for (size_t i = 0, end = num_books; i < end; ++i) {
            sqlite3_step(stmt);
            books[i].name = sprintf_alloc("%s", sqlite3_column_text(stmt, 0));
            books[i].author = sprintf_alloc("%s", sqlite3_column_text(stmt, 1));
        }
        sqlite3_finalize(stmt);
        return num_books;
    }
}
