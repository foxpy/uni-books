add_library(DatabaseWrapper
    database_new.c
    database_open.c
    database_close.c
    schema.c
)
target_link_libraries(DatabaseWrapper PUBLIC qc SQLite::SQLite3)
target_include_directories(DatabaseWrapper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
