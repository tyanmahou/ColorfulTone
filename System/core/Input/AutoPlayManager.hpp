#pragma once
#include <core/Types.hpp>
#include <utils/Singleton/DynamicSingleton.hpp>
#include <memory>

namespace ct
{
    class AutoPlayManager final : protected DynamicSingleton<AutoPlayManager>
    {
        friend class DynamicSingleton<AutoPlayManager>;
    public:

        static void ChangePlayMode();
        static void SetAutoPlay(bool isAuto);
        static bool IsAutoPlay();
        static bool IsRedPressed();
        static bool IsBluePressed();
        static bool IsYellowPressed();
        static void Update();
        static void Input(NoteType type);
    private:
        AutoPlayManager();
    private:
        class Impl;
        std::unique_ptr<Impl> m_pImpl;
    };
}
