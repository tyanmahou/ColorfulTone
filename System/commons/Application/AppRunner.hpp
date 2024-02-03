#pragma once

namespace ct
{
    class IApplication;

    class AppRunner
    {
    public:
        bool run(IApplication* pApp) const;
    };
}
