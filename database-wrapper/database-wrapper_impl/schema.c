char const* schema =
        "CREATE TABLE Users (\n"
        "    user_id INTEGER PRIMARY KEY,\n"
        "    is_admin INTEGER NOT NULL,\n"
        "    username TEXT NOT NULL UNIQUE,\n"
        "    password_hash TEXT NOT NULL\n"
        ");\n"
        "\n"
        "CREATE TABLE Books (\n"
        "    book_id INTEGER PRIMARY KEY,\n"
        "    name TEXT NOT NULL,\n"
        "    author TEXT NOT NULL\n"
        ");\n";
