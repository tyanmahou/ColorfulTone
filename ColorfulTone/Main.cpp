#include <Siv3D.hpp> 
#include "MainApp.hpp"

void Main()
{
    auto* app = new ct::MainApp();
    app->run();

    // NOTE: 謎のバグ対応を試す
    try {
        delete app;
    } catch (...) {

    }
}