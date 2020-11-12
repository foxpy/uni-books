#include <Fl/Fl_Widget.H>
#include <Fl/Fl_Window.H>
#include <Fl/Fl_Menu_Bar.H>
#include <Fl/Fl_Native_File_Chooser.H>

const unsigned WINDOW_SIZE_X = 800;
const unsigned WINDOW_SIZE_Y = 600;

static void menu_bar_new_file_cb(Fl_Widget*, void*) {
    Fl_Native_File_Chooser file_chooser;
    file_chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    if (file_chooser.show() != 0) {
        return;
    }
    char const* picked = file_chooser.filename();
    fl_message("Attempt to save to \"%s\"", picked);
}

static void menu_bar_open_file_cb(Fl_Widget*, void*) {
    Fl_Native_File_Chooser file_chooser;
    file_chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
    if (file_chooser.show() != 0) {
        return;
    }
    char const* picked = file_chooser.filename();
    fl_message("Atttempt to open \"%s\"", picked);
}

static void menu_bar_close_cb(Fl_Widget*, void*) {
    fl_message("Now quiting! TODO: adequate quit");
    exit(EXIT_SUCCESS);
}

int main() {
    Fl_Window auth_window(WINDOW_SIZE_X, WINDOW_SIZE_Y, "Library");
    Fl_Menu_Bar menu_bar(0, 0, WINDOW_SIZE_X, 25);
    menu_bar.add("&File/&New", "^n", menu_bar_new_file_cb);
    menu_bar.add("&File/&Open", "^o", menu_bar_open_file_cb);
    menu_bar.add("&File/&Quit", "^q", menu_bar_close_cb);
    auth_window.end();
    auth_window.show();
    return Fl::run();
}
