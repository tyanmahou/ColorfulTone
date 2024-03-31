#pragma once
#include <utils/Singleton/DynamicSingleton.hpp>
#include <core/Input/IGameInputPlayer.hpp>
#include <memory>

namespace ct
{
    class InputManager final : protected DynamicSingleton<InputManager>
    {
        friend class DynamicSingleton<InputManager>;
    public:
        /// <summary>
        /// 高精度か
        /// </summary>
        /// <returns></returns>
        static bool IsHighPrecision();

        static bool IsRedClicked();
        static bool IsRedPressed();
        static void PopRed();
        static s3d::int64 RedClickedTimeOffset();

        static bool IsBlueClicked();
        static bool IsBluePressed();
        static void PopBlue();
        static s3d::int64 BlueClickedTimeOffset();

        static bool IsYellowClicked();
        static bool IsYellowPressed();
        static void PopYellow();
        static s3d::int64 YellowClickedTimeOffset();

        static bool IsAnyClicked();
        static void Update();

        static void ForceInput(NoteType type);
    private:
        InputManager();

        IGameInputPlayer* getPlayer() const;
    private:
        s3d::Array<std::unique_ptr<IGameInputPlayer>> m_players;
    };
}
