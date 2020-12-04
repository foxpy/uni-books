#include "uni-books.hpp"

UserPanel::UserPanel(int x, int y, MainWindow *m): Fl_Widget(x, y, 700, 500) {
    main_window = m;
    box = new Fl_Box(x, y, 700, 500);
    table = new BooksTable(x, y, main_window);
    box->box(FL_UP_BOX);
    new_book_button = new Fl_Button(x + 500, y + 460, 200, 40, "New book");
    new_book_button->callback(new_book_cb, main_window);
    delete_book_button = new Fl_Button(x + 300, y + 460, 200, 40, "Delete book");
    delete_book_button->callback(delete_book_cb, main_window);
}

UserPanel::~UserPanel() {
    delete box;
    delete table;
    delete new_book_button;
    delete delete_book_button;
}

void UserPanel::draw() {}

void UserPanel::hide() {
    box->hide();
    table->hide();
    new_book_button->hide();
    delete_book_button->hide();
}

void UserPanel::show() {
    box->show();
    table->show();
    new_book_button->show();
    delete_book_button->show();
}

BooksTable::BooksTable(int x, int y, MainWindow *m): Fl_Table(x, y, 700, 460) {
    main_window = m;
    books_count = 0;
    books = nullptr;
    end();
}

BooksTable::~BooksTable() {
    clear_books();
}

void BooksTable::DrawHeader(char const* s, int X, int Y, int W, int H) {
    fl_push_clip(X, Y, W, H);
    fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, row_header_color());
    fl_color(FL_BLACK);
    fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
    fl_pop_clip();
}

void BooksTable::DrawData(char const* s, int X, int Y, int W, int H) {
    fl_push_clip(X, Y, W, H);
    fl_color(FL_WHITE);
    fl_rectf(X, Y, W, H);
    fl_color(FL_GRAY0);
    fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
    fl_color(color());
    fl_rect(X, Y, W, H);
    fl_pop_clip();
}

void BooksTable::draw_cell(TableContext context, int ROW, int COL, int X, int Y, int W, int H) {
    switch (context) {
        case CONTEXT_STARTPAGE:
            fl_font(FL_HELVETICA, 16);
            return;
        case CONTEXT_COL_HEADER:
            char const* header_name;
            if (COL == 0) {
                header_name = "Name";
            } else if (COL == 1) {
                header_name = "Author";
            } else {
                header_name = "error";
            }
            DrawHeader(header_name, X, Y, W, H);
            return;
        case CONTEXT_CELL:
            char const* data;
            if (COL == 0) {
                data = books[ROW].name;
            } else if (COL == 1) {
                data = books[ROW].author;
            } else {
                data = "error";
            }
            DrawData(data, X, Y, W, H);
            return;
        default:
            return;
    }
}

void BooksTable::populate() {
    char* err;
    clear_books();
    ptrdiff_t num_books = database_get_books(main_window->database_handle, &books, &err);
    if (num_books < 0) {
        fl_message("Failed to load books: %s", err);
        free(err);
        return;
    } else {
        this->books_count = num_books;
        rows(num_books);
        row_header(0);
        row_height_all(30);
        row_resize(1);
        cols(2);
        col_header(1);
        col_width_all(200);
        col_resize(1);
    }
}

void BooksTable::clear_books() {
    if (books != nullptr) {
        for (size_t i = 0; i < books_count; ++i) {
            free(books[i].author);
            free(books[i].name);
        }
        free(books);
        books = nullptr;
    }
}

void new_book_cb(Fl_Widget*, void* m) {
    char bookname[81];
    char author[81];
    auto main_window = reinterpret_cast<MainWindow *>(m);
    char const *input_text;
    {
        input_text = fl_input("Book name");
        if (input_text == nullptr) {
            return;
        } else if (strlen(input_text) > 80) {
            fl_message("Book name too long!");
            return;
        }
        strcpy(bookname, input_text);
        input_text = fl_input("Book author");
        if (input_text == nullptr) {
            return;
        } else if (strlen(input_text) > 80) {
            fl_message("Book author too long!");
            return;
        }
        strcpy(author, input_text);
    }
    {
        char* err;
        if (!database_add_book(main_window->database_handle, bookname, author, &err)) {
            fl_message("Failed to add book %s: %s", bookname, err);
            free(err);
        }
    }
    main_window->user_panel->table->populate();
}

void delete_book_cb(Fl_Widget*, void* m) {
    auto main_window = reinterpret_cast<MainWindow*>(m);
    char const* bookname = fl_input("Book name to delete");
    if (bookname == nullptr) {
        return;
    } else {
        char* err;
        if (!database_delete_book(main_window->database_handle, bookname, &err)) {
            fl_message("Failed to delete book %s: %s", bookname, err);
            free(err);
        }
        main_window->user_panel->table->populate();
    }
}
