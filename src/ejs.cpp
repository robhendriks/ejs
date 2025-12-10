#include "app.h"

int main(const int argc, const char **argv) {
    ejs::App app{ejs::AppOptions::getDefault()};
    app.init();

    /*Runtime rt;
    rt.addModulePath("plugins/*", R"(C:\Git\ejs\plugins\*\main.js)");

    const Context ctx{rt};

    const Value example = ctx.evalModule("plugins/foo");
    if (example.isException()) {
        example.printException();
    }

    const Value exampleModule = ctx.getModule("plugins/foo");
    if (exampleModule.isObject()) {
        const Value result = exampleModule["main"]();

        if (result.isException()) {
            result.printException();
        } else {
            spdlog::info("Result: {}", result.as<String>().data());
        }
    }

    spdlog::info("done");*/

    return 0;
}
