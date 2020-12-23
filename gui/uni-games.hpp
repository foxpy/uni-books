#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Table.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Double_Window.H>

extern "C" {
#   include "database-wrapper.h"
}

struct MainWindow;
struct LoginWidget;
struct AdminPanel;
struct UsersTable;
struct UserPanel;
struct GamesTable;
struct RestoreGamesTable;

void menu_file_new_cb(Fl_Widget*, void*);
void menu_file_open_cb(Fl_Widget*, void*);
void menu_file_close_cb(Fl_Widget*, void*);
void menu_file_quit_cb(Fl_Widget*, void*);
void menu_help_about_cb(Fl_Widget*, void*);
void authorize_cb(Fl_Widget*, void*);
void new_user_cb(Fl_Widget*, void*);
void delete_user_cb(Fl_Widget*, void*);
void new_game_cb(Fl_Widget*, void*);
void delete_game_cb(Fl_Widget*, void*);
void restore_games_button_cb(Fl_Widget*, void*);
void go_back_button_cb(Fl_Widget*, void*);
void restore_game_button_cb(Fl_Widget*, void*);

struct MainWindow: Fl_Double_Window {
    MainWindow(int w, int h, char const* t);
    ~MainWindow() override;
    db* database_handle;
    Fl_Menu_Bar* menu_bar;
    LoginWidget* login_widget;
    AdminPanel* admin_panel;
    UserPanel* user_panel;
};

struct LoginWidget: Fl_Widget {
    LoginWidget(int x, int y, MainWindow* m);
    ~LoginWidget() override;
    void draw() override;
    void hide() override;
    void show() override;
    MainWindow* main_window;
    Fl_Box* box;
    Fl_Box* label;
    Fl_Input* username_input;
    Fl_Secret_Input* password_input;
    Fl_Button* authorize_button;
};

struct AdminPanel: Fl_Widget {
    AdminPanel(int x, int y, MainWindow* m);
    ~AdminPanel() override;
    void draw() override;
    void hide() override;
    void show() override;
    MainWindow* main_window;
    Fl_Box* box;
    UsersTable* table;
    Fl_Button* new_user_button;
    Fl_Button* delete_user_button;
};

struct UsersTable: Fl_Table {
    UsersTable(int x, int y, MainWindow* m);
    ~UsersTable();
    void DrawHeader(char const* s, int X, int Y, int W, int H);
    void DrawData(char const* s, int X, int Y, int W, int H);
    void draw_cell(TableContext context, int ROW=0, int COL=0, int X=0, int Y=0, int W=0, int H=0) override;
    void populate();
    void clear_users();
    MainWindow* main_window;
    size_t users_count;
    struct user* users;
};

struct UserPanel: Fl_Widget {
    UserPanel(int x, int y, MainWindow* m);
    ~UserPanel() override;
    void draw() override;
    void hide() override;
    void show() override;
    MainWindow* main_window;
    Fl_Box* box;
    GamesTable* table;
    RestoreGamesTable* restore_table;
    Fl_Button* restore_games_button;
    Fl_Button* go_back_button;
    Fl_Button* restore_game_button;
    Fl_Button* new_game_button;
    Fl_Button* delete_game_button;
};

struct GamesTable: Fl_Table {
    GamesTable(int x, int y, MainWindow* m);
    ~GamesTable() override;
    void DrawHeader(char const* s, int X, int Y, int W, int H);
    void DrawData(char const* s, int X, int Y, int W, int H);
    void draw_cell(TableContext context, int ROW=0, int COL=0, int X=0, int Y=0, int W=0, int H=0) override;
    void populate();
    void clear_games();
    MainWindow* main_window;
    size_t games_count;
    struct game* games;
};

struct RestoreGamesTable: Fl_Table {
    RestoreGamesTable(int x, int y, MainWindow* m);
    ~RestoreGamesTable() override;
    void DrawHeader(char const* s, int X, int Y, int W, int H);
    void DrawData(char const* s, int X, int Y, int W, int H);
    void draw_cell(TableContext context, int ROW=0, int COL=0, int X=0, int Y=0, int W=0, int H=0) override;
    void populate();
    void clear_games();
    MainWindow* main_window;
    size_t games_count;
    struct game* games;
};
