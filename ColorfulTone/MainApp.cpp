#include "MainApp.hpp"
#include <commons/Game/Game.hpp>
#include <commons/Constants.hpp>
#include <Siv3D.hpp>

namespace ct
{
    MainApp::MainApp():
        BaseApp(Constants::AppName, Constants::AppResolution, true)
    {
    }
    MainApp::~MainApp()
    {
    }
    void MainApp::onStartup()
    {
        Game::Startup();
    }
    bool MainApp::onUpdate()
    {
        this->preUpdate();

        return Game::Update();
    }
    void MainApp::onShutdown()
    {
        Game::Shutdown();
    }
}
