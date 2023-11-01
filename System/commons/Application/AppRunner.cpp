#include <commons/Application/AppRunner.hpp>
#include <commons/Application/base/IApplication.hpp>

namespace ct
{
    bool AppRunner::run(IApplication* pApp) const
    {
        pApp->onStartup();
        while (System::Update()) {
            if (!pApp->onUpdate()) {
                break;
            }
        }
        pApp->onShutdown();
        return true;
    }
}
