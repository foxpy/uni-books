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

static void menu_file_new_cb(Fl_Widget*, void*);
static void menu_file_open_cb(Fl_Widget*, void*);
static void menu_file_close_cb(Fl_Widget*, void*);
static void menu_file_quit_cb(Fl_Widget*, void*);
static void menu_help_about_cb(Fl_Widget*, void*);
static void authorize_cb(Fl_Widget*, void*);

struct LoginWidget: Fl_Widget {
    LoginWidget(int x, int y, MainWindow* m): Fl_Widget(x, y, 400, 200) {
        main_window = m;
        box = new Fl_Box(x, y, 400, 200);
        box->box(FL_UP_BOX);
        label = new Fl_Box(x+20, y+20, 360, 20, "Log in");
        username_input = new Fl_Input(x+80, y+40, 300, 40, "Username");
        password_input = new Fl_Input(x+80, y+100, 300, 40, "Password");
        authorize_button = new Fl_Button(x+150, y+150, 100, 40, "Authorize");
        authorize_button->callback(authorize_cb, this);
    }
    ~LoginWidget() override {
        delete box;
        delete label;
        delete username_input;
        delete password_input;
        delete authorize_button;
    }
    void draw() override {}
    void hide() override {
        box->hide();
        label->hide();
        username_input->hide();
        password_input->hide();
        authorize_button->hide();
    }
    void show() override {
        box->show();
        label->show();
        username_input->show();
        password_input->show();
        authorize_button->show();
    }
    MainWindow* main_window;
    Fl_Box* box;
    Fl_Box* label;
    Fl_Input* username_input;
    Fl_Input* password_input;
    Fl_Button* authorize_button;
};

struct MainWindow: Fl_Double_Window {
    MainWindow(int w, int h, char const* t): Fl_Double_Window(w, h, t) {
        database_handle = nullptr;
        menu_bar = new Fl_Menu_Bar(0, 0, w, 25);
        menu_bar->add("&File/&New", "^n", menu_file_new_cb, this);
        menu_bar->add("&File/&Open", "^o", menu_file_open_cb, this);
        menu_bar->add("&File/&Close", "^w", menu_file_close_cb, this);
        menu_bar->add("&File/&Quit", "^q", menu_file_quit_cb, this);
        menu_bar->add("&Help/&About", "^/", menu_help_about_cb, this);
        login_widget = new LoginWidget(200, 200, this);
        login_widget->hide();
    }
    ~MainWindow() override {
        delete menu_bar;
        delete login_widget;
    }
    db* database_handle;
    Fl_Menu_Bar* menu_bar;
    LoginWidget* login_widget;
};

static void menu_file_new_cb(Fl_Widget*, void* m) {
    auto* main_window = reinterpret_cast<MainWindow*>(m);
    Fl_Native_File_Chooser file_chooser;
    file_chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    file_chooser.preset_file("library.db");
    if (file_chooser.show() != 0) {
        return;
    }
    char const* picked = file_chooser.filename();
    char const* admin_password;
ask_for_password:
    admin_password = fl_input("Admin password");
    if (admin_password == nullptr) {
        return;
    }
    if (strlen(admin_password) < 8) {
        fl_message("Admin password should have at least 8 characters");
        goto ask_for_password;
    }
    char* err;
    if (!database_new(picked, &err, admin_password)) {
        fl_message("%s", err);
    } else {
        fl_message("Created database %s", picked);
    }
    main_window->database_handle = database_open(picked, &err);
    if (main_window->database_handle == nullptr) {
        fl_message("Failed to open database: %s", err);
        free(err);
    }
}

static void menu_file_open_cb(Fl_Widget* w, void* m) {
    auto* main_window = reinterpret_cast<MainWindow*>(m);
    menu_file_close_cb(w, m);
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
    main_window->login_widget->show();
}

static void menu_file_close_cb(Fl_Widget*, void* m) {
    auto* main_window = reinterpret_cast<MainWindow*>(m);
    if (main_window->database_handle != nullptr) {
        database_close(main_window->database_handle);
        main_window->database_handle = nullptr;
    }
    main_window->login_widget->hide();
}

static void menu_file_quit_cb(Fl_Widget* w, void* m) {
    menu_file_close_cb(w, m);
    exit(EXIT_SUCCESS);
}

static void menu_help_about_cb(Fl_Widget*, void*) {
    fl_message("There is no help");
}

static void authorize_cb(Fl_Widget*, void* l) {
    auto login_widget = reinterpret_cast<LoginWidget*>(l);
    char const* username = login_widget->username_input->value();
    char const* password = login_widget->password_input->value();
    if (username != nullptr && password != nullptr) {
        char* err = nullptr;
        db_user_type type = database_check_credentials(
                login_widget->main_window->database_handle, username, password, &err);
        if (type == DATABASE_UNAUTHORIZED) {
            fl_message("Failed to authorize: %s", err);
            free(err);
        } else {
            fl_message("Successfully authorized as %s", type == DATABASE_ADMIN ? "admin" : "casual user");
        }
    }
}

int main() {
    MainWindow main_window(800, 600, "Library");
    main_window.end();
    main_window.show();
    return Fl::run();
}
