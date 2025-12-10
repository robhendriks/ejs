#include "app.h"

int main(const int argc, const char **argv) {
    ejs::App app{ejs::AppOptions::getDefault()};
    app.init();

    return 0;
}
