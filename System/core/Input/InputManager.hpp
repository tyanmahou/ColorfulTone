#pragma once
#include <utils/Singleton/DynamicSingleton.hpp>
#include <memory>

namespace ct
{
    class InputManager final : protected DynamicSingleton<InputManager>
    {
        friend class DynamicSingleton<InputManager>;
    public:

        static bool IsRedClicked();
        static void PopRed();
        static s3d::int64 RedClickedTimeOffset();

        static bool IsBlueClicked();
        static void PopBlue();
        static s3d::int64 BlueClickedTimeOffset();

        static bool IsYellowClicked();
        static void PopYellow();
        static s3d::int64 YellowClickedTimeOffset();

        static bool IsAnyClicked();
        static void Update();
    private:
        InputManager();
    private:
        class Impl;
        std::unique_ptr<Impl> m_pImpl;
    };
}
