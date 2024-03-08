#pragma once
#include <utils/Singleton/Singleton.hpp>

namespace ct
{
    /// <summary>
    /// 練習マネージャー
    /// </summary>
    class PracticeManager final : Singleton<PracticeManager>
    {
        friend class Singleton<PracticeManager>;
    public:
        static void ChangePlayMode();
        static void SetPracticeMode(bool isPractice);
        static bool IsPractice();
    private:
        bool m_isPractice = false;
    };
}