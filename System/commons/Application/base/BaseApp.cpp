#include <commons/Application/base/BaseApp.hpp>
#include <commons/Application/AppRunner.hpp>
#include <Siv3D.hpp>

namespace ct
{
    BaseApp::BaseApp(const s3d::String& appName, s3d::Size windowSize, bool useScalable):
        m_windowCtrl(appName, windowSize, useScalable)
    {}
    bool BaseApp::run()
    {
        AppRunner runner;
        return runner.run(this);
    }
}
