#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Double_Window.H>

extern "C" {
#   include "database-wrapper.h"
}

struct MainWindow;
struct LoginWidget;

void menu_file_new_cb(Fl_Widget*, void*);
void menu_file_open_cb(Fl_Widget*, void*);
void menu_file_close_cb(Fl_Widget*, void*);
void menu_file_quit_cb(Fl_Widget*, void*);
void menu_help_about_cb(Fl_Widget*, void*);
void authorize_cb(Fl_Widget*, void*);

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

struct MainWindow: Fl_Double_Window {
    MainWindow(int w, int h, char const* t);
    ~MainWindow() override;
    db* database_handle;
    Fl_Menu_Bar* menu_bar;
    LoginWidget* login_widget;
};
