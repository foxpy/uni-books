#include "uni-books.hpp"

AdminPanel::AdminPanel(int x, int y, MainWindow* m): Fl_Widget(x, y, 700, 500) {
    main_window = m;
    box = new Fl_Box(x, y, 700, 500);
    table = new UsersTable(x, y, main_window);
    box->box(FL_UP_BOX);
}

AdminPanel::~AdminPanel() {
    delete box;
    delete table;
}

void AdminPanel::draw() {}

void AdminPanel::hide() {
    box->hide();
    table->hide();
}

void AdminPanel::show() {
    box->show();
    table->show();
}

UsersTable::UsersTable(int x, int y, MainWindow* m): Fl_Table(x, y, 700, 460) {
    main_window = m;
    users_count = 0;
    users = nullptr;
}

UsersTable::~UsersTable() {
    clear_users();
}

void UsersTable::DrawHeader(char const* s, int X, int Y, int W, int H) {
    fl_push_clip(X, Y, W, H);
    fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
    fl_color(FL_BLACK);
    fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
    fl_pop_clip();
}

void UsersTable::DrawData(char const* s, int X, int Y, int W, int H) {
    fl_push_clip(X, Y, W, H);
    fl_color(FL_WHITE);
    fl_rectf(X, Y, W, H);
    fl_color(FL_GRAY0);
    fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
    fl_color(color());
    fl_rect(X, Y, W, H);
    fl_pop_clip();
}

void UsersTable::draw_cell(TableContext context, int ROW, int COL, int X, int Y, int W, int H) {
    switch (context) {
        case CONTEXT_STARTPAGE:
            fl_font(FL_HELVETICA, 16);
            return;
        case CONTEXT_COL_HEADER:
            char const* col_name;
            if (COL == 0) {
                col_name = "name";
            } else if (COL == 1) {
                col_name = "type";
            } else {
                col_name = "error";
            }
            DrawHeader(col_name, X, Y, W, H);
            return;
        case CONTEXT_CELL:
            char const* data;
            if (COL == 0) {
                data = users[ROW].username;
            } else if (COL == 1) {
                data = users[ROW].is_admin ? "admin" : "user";
            } else {
                data = "error";
            }
            DrawData(data, X, Y, W, H);
            return;
        default:
            return;
    }
}

void UsersTable::clear_users() {
    if (users != nullptr) {
        for (size_t i = 0; i < users_count; ++i) {
            free (users[i].username);
            free (users[i].password_hash);
        }
        free(users);
        users = nullptr;
    }
}

void UsersTable::populate() {
    char* err;
    clear_users();
    ptrdiff_t num_users = database_get_users(main_window->database_handle, &users, &err);
    if (num_users < 0) {
        fl_message("Failed to load users: %s", err);
        free(err);
        return;
    } else if (num_users == 0) {
        return;
    } else {
        this->users_count = num_users;
        rows(num_users);
        row_header(0);
        row_height_all(30);
        row_resize(1);
        cols(2);
        col_header(1);
        col_width_all(200);
        col_resize(1);
        end();
    }
}
