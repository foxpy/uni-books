#include <Fl/Fl_Widget.H>
#include <Fl/Fl_Window.H>
#include <Fl/Fl_Menu_Bar.H>
#include <Fl/Fl_Native_File_Chooser.H>
#include <Fl/Fl_Double_Window.H>
extern "C" {
#   include "database-wrapper.h"
}

const unsigned WINDOW_SIZE_W = 800;
const unsigned WINDOW_SIZE_H = 600;

static void menu_file_new_cb(Fl_Widget*, void*);
static void menu_file_open_cb(Fl_Widget*, void*);
static void menu_file_quit_cb(Fl_Widget*, void*);
static void menu_help_about_cb(Fl_Widget*, void*);

struct MainWindow: Fl_Double_Window {
    MainWindow(int w, int h, char const* t): Fl_Double_Window(w, h, t) {
        database_handle = nullptr;
        menu_bar = new Fl_Menu_Bar(0, 0, w, 25);
        menu_bar->add("&File/&New", "^n", menu_file_new_cb, this);
        menu_bar->add("&File/&Open", "^o", menu_file_open_cb, this);
        menu_bar->add("&File/&Quit", "^q", menu_file_quit_cb, this);
        menu_bar->add("&Help/&About", "^/", menu_help_about_cb, this);
    }
    ~MainWindow() override {
        delete menu_bar;
    }
    db* database_handle;
    Fl_Menu_Bar* menu_bar;
};

static void menu_file_new_cb(Fl_Widget*, void*) {
    Fl_Native_File_Chooser file_chooser;
    file_chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    file_chooser.preset_file("library.db");
    if (file_chooser.show() != 0) {
        return;
    }
    char const* picked = file_chooser.filename();
    char* err;
    if (!database_new(picked, &err)) {
        fl_message("%s", err);
    } else {
        fl_message("Created database %s", picked);
    }
}

static void menu_file_open_cb(Fl_Widget*, void* m) {
    auto* main_window = reinterpret_cast<MainWindow*>(m);
    if (main_window->database_handle != nullptr) {
        database_close(main_window->database_handle);
    }
    Fl_Native_File_Chooser file_chooser;
    file_chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
    if (file_chooser.show() != 0) {
        return;
    }
    char const* picked = file_chooser.filename();
    char* err;
    main_window->database_handle = database_open(picked, &err);
    if (main_window->database_handle == nullptr) {
        fl_message("Failed to open database: %s", err);
        free(err);
    }
}

static void menu_file_quit_cb(Fl_Widget*, void* m) {
    auto* main_window = reinterpret_cast<MainWindow*>(m);
    if (main_window->database_handle != nullptr) {
        database_close(main_window->database_handle);
    }
    exit(EXIT_SUCCESS);
}

static void menu_help_about_cb(Fl_Widget*, void*) {
    fl_message("There is no help");
}

int main() {
    MainWindow main_window(WINDOW_SIZE_W, WINDOW_SIZE_H, "Library");
    main_window.end();
    main_window.show();
    return Fl::run();
}
