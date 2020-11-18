#include "database-wrapper.h"

char const* schema =
        "CREATE TABLE Users (\n"
        "    user_id INTEGER PRIMARY KEY,\n"
        "    username TEXT NOT NULL,\n"
        "    password_salt TEXT NOT NULL,\n"
        "    password_hash TEXT NOT NULL\n"
        ");\n"
        "\n"
        "CREATE TABLE People (\n"
        "    human_id INTEGER PRIMARY KEY,\n"
        "    first_name TEXT NOT NULL,\n"
        "    last_name TEXT NOT NULL,\n"
        "    middle_name TEXT,\n"
        "    address TEXT NOT NULL,\n"
        "    passport_number INTEGER NOT NULL,\n"
        "    birthday_date TEXT NOT NULL\n"
        ");\n";
