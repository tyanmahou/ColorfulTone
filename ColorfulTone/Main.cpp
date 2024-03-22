#include <Siv3D.hpp> 
#include "MainApp.hpp"

void Main()
{
    auto app = std::make_unique<ct::MainApp>();
    app->run();
}