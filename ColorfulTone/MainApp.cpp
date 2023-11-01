#include "MainApp.hpp"
#include <commons/Game/Game.hpp>
#include <commons/Constants.hpp>

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
        if (KeyF4.down()) {
            m_windowCtrl.changeWindowSizeNext();
        }
        return Game::Update();
    }
    void MainApp::onShutdown()
    {
        Game::Shutdown();
    }
}
