#include <core/Play/Context/PlayContext.hpp>

namespace ct
{
    void PlayContext::ChangeAutoPlay()
    {
        SetAutoPlay(!IsAutoPlay());
    }
    void PlayContext::SetAutoPlay(bool isAuto)
    {
        Instance()->m_isAutoPlay = isAuto;
    }
    bool PlayContext::IsAutoPlay()
    {
        return Instance()->m_isAutoPlay;
    }
    void PlayContext::ChangePracticePlay()
    {
        SetPracticePlay(!IsPracticePlay());
    }
    void PlayContext::SetPracticePlay(bool isPractice)
    {
        Instance()->m_isPractice = isPractice;
    }
    bool PlayContext::IsPracticePlay()
    {
        return Instance()->m_isPractice;
    }
    void PlayContext::Revert()
    {
        SetAutoPlay(false);
        SetPracticePlay(false);
    }
    bool PlayContext::CanNotUpdateScore()
    {
        return IsAutoPlay() || IsPracticePlay();
    }
}
