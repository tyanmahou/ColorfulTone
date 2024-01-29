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
        if (KeyF4.down()) {
            m_windowCtrl.changeWindowSizeNext();
        }
        if (KeyF7.down()) {
            m_showFps ^= true;
        }
        if (m_showFps) {
            PutText(U"{}"_fmt(Profiler::FPS()), Arg::topLeft = Vec2{ 0, 0 });
        }

        return Game::Update();
    }
    void MainApp::onShutdown()
    {
        Game::Shutdown();
    }
}
