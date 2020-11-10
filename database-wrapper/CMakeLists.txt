add_library(DatabaseWrapper
    database_new.c
)
target_include_directories(DatabaseWrapper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
