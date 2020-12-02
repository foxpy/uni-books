#include "uni-books.hpp"

AdminPanel::AdminPanel(int x, int y, MainWindow* m): Fl_Widget(x, y, 700, 500) {
    main_window = m;
    box = new Fl_Box(x, y, 700, 500);
    box->box(FL_UP_BOX);
}

AdminPanel::~AdminPanel() {
    delete box;
}

void AdminPanel::draw() {}

void AdminPanel::hide() {
    box->hide();
}

void AdminPanel::show() {
    box->show();
}
