add_library(DatabaseWrapper
    database_new.c
    database_open.c
    database_close.c
    schema.c
    database-wrapper_impl/hash_password.c
    database-wrapper_impl/check_password.c
)
target_link_libraries(DatabaseWrapper PUBLIC qc SQLite::SQLite3 unofficial-sodium::sodium)
target_include_directories(DatabaseWrapper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
