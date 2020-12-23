#include "uni-games.hpp"

UserPanel::UserPanel(int x, int y, MainWindow *m): Fl_Widget(x, y, 700, 500) {
    main_window = m;
    box = new Fl_Box(x, y, 700, 500);
    table = new GamesTable(x, y, main_window);
    restore_table = new RestoreGamesTable(x, y, main_window);
    box->box(FL_UP_BOX);
    restore_games_button = new Fl_Button(x + 0, y + 460, 200, 40, "Restore games");
    restore_games_button->callback(restore_games_button_cb, main_window);
    go_back_button = new Fl_Button(x + 0, y + 460, 200, 40, "Manage games");
    go_back_button->callback(go_back_button_cb, main_window);
    restore_game_button = new Fl_Button(x + 500, y + 460, 200, 40, "Restore game");
    restore_game_button->callback(restore_game_button_cb, main_window);
    new_game_button = new Fl_Button(x + 500, y + 460, 200, 40, "New game");
    new_game_button->callback(new_game_cb, main_window);
    delete_game_button = new Fl_Button(x + 300, y + 460, 200, 40, "Delete game");
    delete_game_button->callback(delete_game_cb, main_window);
}

UserPanel::~UserPanel() {
    delete box;
    delete table;
    delete restore_table;
    delete restore_games_button;
    delete restore_game_button;
    delete new_game_button;
    delete delete_game_button;
}

void UserPanel::draw() {}

void UserPanel::hide() {
    box->hide();
    table->hide();
    restore_table->hide();
    restore_games_button->hide();
    restore_game_button->hide();
    go_back_button->hide();
    new_game_button->hide();
    delete_game_button->hide();
}

void UserPanel::show() {
    box->show();
    table->show();
    restore_table->hide();
    restore_games_button->show();
    restore_game_button->hide();
    go_back_button->hide();
    new_game_button->show();
    delete_game_button->show();
}

GamesTable::GamesTable(int x, int y, MainWindow *m): Fl_Table(x, y, 700, 460) {
    main_window = m;
    games_count = 0;
    games = nullptr;
    end();
}

GamesTable::~GamesTable() {
    clear_games();
}

void GamesTable::DrawHeader(char const* s, int X, int Y, int W, int H) {
    fl_push_clip(X, Y, W, H);
    fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
    fl_color(FL_BLACK);
    fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
    fl_pop_clip();
}

void GamesTable::DrawData(char const* s, int X, int Y, int W, int H) {
    fl_push_clip(X, Y, W, H);
    fl_color(FL_WHITE);
    fl_rectf(X, Y, W, H);
    fl_color(FL_GRAY0);
    fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
    fl_color(color());
    fl_rect(X, Y, W, H);
    fl_pop_clip();
}

void GamesTable::draw_cell(TableContext context, int ROW, int COL, int X, int Y, int W, int H) {
    switch (context) {
        case CONTEXT_STARTPAGE:
            fl_font(FL_HELVETICA, 16);
            return;
        case CONTEXT_COL_HEADER:
            char const* header_name;
            if (COL == 0) {
                header_name = "Name";
            } else if (COL == 1) {
                header_name = "Publisher";
            } else {
                header_name = "error";
            }
            DrawHeader(header_name, X, Y, W, H);
            return;
        case CONTEXT_CELL:
            char const* data;
            if (COL == 0) {
                data = games[ROW].name;
            } else if (COL == 1) {
                data = games[ROW].publisher;
            } else {
                data = "error";
            }
            DrawData(data, X, Y, W, H);
            return;
        default:
            return;
    }
}

void GamesTable::populate() {
    char* err;
    clear_games();
    ptrdiff_t num_games = database_get_games(main_window->database_handle, false, &games, &err);
    if (num_games < 0) {
        fl_message("Failed to load games: %s", err);
        free(err);
        return;
    } else {
        this->games_count = num_games;
        rows(num_games);
        row_header(0);
        row_height_all(30);
        row_resize(1);
        cols(2);
        col_header(1);
        col_width_all(200);
        col_resize(1);
    }
}

void GamesTable::clear_games() {
    if (games != nullptr) {
        for (size_t i = 0; i < games_count; ++i) {
            free(games[i].publisher);
            free(games[i].name);
        }
        free(games);
        games = nullptr;
    }
}

void new_game_cb(Fl_Widget*, void* m) {
    char gamename[81];
    char publisher[81];
    auto main_window = reinterpret_cast<MainWindow *>(m);
    char const *input_text;
    {
        input_text = fl_input("Game name");
        if (input_text == nullptr) {
            return;
        } else if (strlen(input_text) > 80) {
            fl_message("Game name too long!");
            return;
        }
        strcpy(gamename, input_text);
        input_text = fl_input("Game publisher");
        if (input_text == nullptr) {
            return;
        } else if (strlen(input_text) > 80) {
            fl_message("Game publisher name is too long!");
            return;
        }
        strcpy(publisher, input_text);
    }
    {
        char* err;
        if (!database_add_game(main_window->database_handle, gamename, publisher, &err)) {
            fl_message("Failed to add game %s: %s", gamename, err);
            free(err);
        }
    }
    main_window->user_panel->table->populate();
}

void delete_game_cb(Fl_Widget*, void* m) {
    auto main_window = reinterpret_cast<MainWindow*>(m);
    char const* gamename = fl_input("Game name to delete");
    if (gamename == nullptr) {
        return;
    } else {
        char* err;
        if (!database_delete_game(main_window->database_handle, gamename, &err)) {
            fl_message("Failed to delete game %s: %s", gamename, err);
            free(err);
        }
        main_window->user_panel->table->populate();
    }
}

RestoreGamesTable::RestoreGamesTable(int x, int y, MainWindow *m): Fl_Table(x, y, 700, 460) {
    main_window = m;
    games_count = 0;
    games = nullptr;
    end();
}

RestoreGamesTable::~RestoreGamesTable() {
    clear_games();
}

void RestoreGamesTable::DrawHeader(char const* s, int X, int Y, int W, int H) {
    fl_push_clip(X, Y, W, H);
    fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
    fl_color(FL_BLACK);
    fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
    fl_pop_clip();
}

void RestoreGamesTable::DrawData(char const* s, int X, int Y, int W, int H) {
    fl_push_clip(X, Y, W, H);
    fl_color(FL_WHITE);
    fl_rectf(X, Y, W, H);
    fl_color(FL_GRAY0);
    fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
    fl_color(color());
    fl_rect(X, Y, W, H);
    fl_pop_clip();
}

void RestoreGamesTable::draw_cell(TableContext context, int ROW, int COL, int X, int Y, int W, int H) {
    switch (context) {
        case CONTEXT_STARTPAGE:
            fl_font(FL_HELVETICA, 16);
            return;
        case CONTEXT_COL_HEADER:
            char const* header_name;
            if (COL == 0) {
                header_name = "Name";
            } else if (COL == 1) {
                header_name = "Publisher";
            } else {
                header_name = "error";
            }
            DrawHeader(header_name, X, Y, W, H);
            return;
        case CONTEXT_CELL:
            char const* data;
            if (COL == 0) {
                data = games[ROW].name;
            } else if (COL == 1) {
                data = games[ROW].publisher;
            } else {
                data = "error";
            }
            DrawData(data, X, Y, W, H);
            return;
        default:
            return;
    }
}

void RestoreGamesTable::populate() {
    char* err;
    clear_games();
    ptrdiff_t num_games = database_get_games(main_window->database_handle, true, &games, &err);
    if (num_games < 0) {
        fl_message("Failed to load games: %s", err);
        free(err);
        return;
    } else {
        this->games_count = num_games;
        rows(num_games);
        row_header(0);
        row_height_all(30);
        row_resize(1);
        cols(2);
        col_header(1);
        col_width_all(200);
        col_resize(1);
    }
}

void RestoreGamesTable::clear_games() {
    if (games != nullptr) {
        for (size_t i = 0; i < games_count; ++i) {
            free(games[i].publisher);
            free(games[i].name);
        }
        free(games);
        games = nullptr;
    }
}

void restore_games_button_cb(Fl_Widget*, void* m) {
    auto main_window = reinterpret_cast<MainWindow*>(m);
    main_window->user_panel->table->hide();
    main_window->user_panel->restore_games_button->hide();
    main_window->user_panel->delete_game_button->hide();
    main_window->user_panel->new_game_button->hide();
    main_window->user_panel->restore_table->show();
    main_window->user_panel->restore_table->populate();
    main_window->user_panel->go_back_button->show();
    main_window->user_panel->restore_game_button->show();
}

void go_back_button_cb(Fl_Widget*, void* m) {
    auto main_window = reinterpret_cast<MainWindow*>(m);
    main_window->user_panel->show();
    main_window->user_panel->table->populate();
}

void restore_game_button_cb(Fl_Widget*, void* m) {
    auto main_window = reinterpret_cast<MainWindow*>(m);
    char const* gamename = fl_input("Game name to restore");
    if (gamename == nullptr) {
        return;
    } else {
        char* err;
        if (!database_restore_game(main_window->database_handle, gamename, &err)) {
            fl_message("Failed to restore game %s: %s", gamename, err);
            free(err);
        }
        main_window->user_panel->restore_table->populate();
    }
}
