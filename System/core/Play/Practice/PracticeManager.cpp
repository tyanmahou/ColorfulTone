#include <core/Play/Practice/PracticeManager.hpp>

namespace ct
{
    void PracticeManager::ChangePlayMode()
    {
        bool& m_isPractice = Instance()->m_isPractice;
        m_isPractice = !m_isPractice;
    }
    void PracticeManager::SetPracticeMode(bool isPractice)
    {
        Instance()->m_isPractice = isPractice;
    }
    bool PracticeManager::IsPractice()
    {
        return Instance()->m_isPractice;
    }
}
