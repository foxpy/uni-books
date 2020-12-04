#include "uni-books.hpp"

UserPanel::UserPanel(int x, int y, MainWindow *m): Fl_Widget(x, y, 700, 500) {
    main_window = m;
    box = new Fl_Box(x, y, 700, 500);
    box->box(FL_UP_BOX);
}

UserPanel::~UserPanel() {
    delete box;
}

void UserPanel::draw() {}

void UserPanel::hide() {
    box->hide();
}

void UserPanel::show() {
    box->show();
}
