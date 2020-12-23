#include "uni-games.hpp"

int main() {
    MainWindow main_window(800, 600, "Games");
    main_window.end();
    main_window.show();
    return Fl::run();
}
