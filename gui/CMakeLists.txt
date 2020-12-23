add_executable(uni-games
    main.cpp
    login-widget.cpp
    main-window.cpp
    admin-panel.cpp
    user-panel.cpp
)
target_link_libraries(uni-games PUBLIC fltk DatabaseWrapper)
