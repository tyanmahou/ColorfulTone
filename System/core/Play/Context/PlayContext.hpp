#pragma once
#include <utils/Singleton/Singleton.hpp>

namespace ct
{
    /// <summary>
    /// コンテキスト
    /// </summary>
    class PlayContext final : Singleton<PlayContext>
    {
        friend class Singleton<PlayContext>;
    public:
        // オートプレイ
        static void ChangeAutoPlay();
        static void SetAutoPlay(bool isAuto);
        static bool IsAutoPlay();

        // 練習モード
        static void ChangePracticePlay();
        static void SetPracticePlay(bool isPractice);
        static bool IsPracticePlay();

    private:
        bool m_isAutoPlay = false;
        bool m_isPractice = false;
    };
}