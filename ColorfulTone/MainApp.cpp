#include "MainApp.hpp"

namespace ct
{
    MainApp::MainApp():
        BaseApp(U"ColorfulTone", Size{ 800, 600 }, true)
    {
    }
    MainApp::~MainApp()
    {
    }
    void MainApp::onStartup()
    {
    }
    bool MainApp::onUpdate()
    {
        if (KeyF4.down()) {
            m_windowCtrl.changeWindowSizeNext();
        }
        Rect(0, 0, 100, 100).draw();
        return true;
    }
    void MainApp::onShutdown()
    {
    }
}
