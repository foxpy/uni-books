#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
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

void menu_file_new_cb(Fl_Widget*, void*);
void menu_file_open_cb(Fl_Widget*, void*);
void menu_file_close_cb(Fl_Widget*, void*);
void menu_file_quit_cb(Fl_Widget*, void*);
void menu_help_about_cb(Fl_Widget*, void*);
void authorize_cb(Fl_Widget*, void*);

struct MainWindow: Fl_Double_Window {
    MainWindow(int w, int h, char const* t);
    ~MainWindow() override;
    db* database_handle;
    Fl_Menu_Bar* menu_bar;
    LoginWidget* login_widget;
    AdminPanel* admin_panel;
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
    Fl_Input* password_input;
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
};

struct UsersTable: Fl_Table {
    UsersTable(int x, int y, MainWindow* m);
    ~UsersTable();
    void DrawHeader(char const* s, int X, int Y, int W, int H);
    void DrawData(char const* s, int X, int Y, int W, int H);
    void draw_cell(TableContext context, int ROW=0, int COL=0, int X=0, int Y=0, int W=0, int H=0);
    void populate();
    void clear_users();
    MainWindow* main_window;
    size_t users_count;
    struct user* users;
};