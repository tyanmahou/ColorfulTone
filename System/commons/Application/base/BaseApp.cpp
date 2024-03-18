#include <commons/Application/base/BaseApp.hpp>
#include <commons/Application/AppRunner.hpp>
#include <Siv3D.hpp>

namespace ct
{
    BaseApp::BaseApp(const s3d::String& appName, s3d::Size windowSize, bool useScalable):
        m_windowCtrl(appName, windowSize, useScalable)
    {

        LicenseManager::SetApplicationLicense(appName, {
            .title = appName,
            .copyright = U"Copyright (c) 2016-2023 Tyanmahou",
            .text = U""
            });
    }
    bool BaseApp::run()
    {
        AppRunner runner;
        return runner.run(this);
    }
    void BaseApp::preUpdate()
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
    }
}
