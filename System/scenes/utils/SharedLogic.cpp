#include <scenes/utils/SharedLogic.hpp>
#include <utils/Input/InputUtl.hpp>
#include <core/Input/PlayKey.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace ct::SharedLogic
{
    s3d::int32 MoveSelectH()
    {
        if (InputUtil::AccelPressed(PlayKey::Left())) {
            return -1;
        }
        if (InputUtil::AccelPressed(PlayKey::Right())) {
            return 1;
        }
        return 0;
    }

    s3d::int32 MoveSelectV()
    {
        if (InputUtil::AccelPressed(PlayKey::Down())) {
            return -1;
        }
        if (InputUtil::AccelPressed(PlayKey::Up())) {
            return 1;
        }
        return 0;
    }
    s3d::int32 MoveSelectV(bool& playSe)
    {
        if (InputUtil::AccelPressed(PlayKey::Down())) {
            playSe = true;
            return -1;
        }
        if (InputUtil::AccelPressed(PlayKey::Up())) {
            playSe = true;
            return 1;
        }
        return 0;
    }
    bool ChangeAutoPlay()
    {
        if (KeyF1.down()) {
            PlayContext::ChangeAutoPlay();
            SoundManager::PlaySe(U"desisionSmall");
            return true;
        }
        return false;
    }
    bool ChangePracticePlay()
    {
        if (KeyF12.down()) {
            PlayContext::ChangePracticePlay();
            SoundManager::PlaySe(U"desisionSmall");
            return true;
        }
        return false;
    }
    bool ChangeLifeGauge()
    {
        if ((s3d::Key3 | s3d::KeyNum3).down()) {
            auto curt = Game::Config().m_lifeGauge;
            int i = static_cast<int32>(curt);
            ++i;
            if (i > static_cast<int32>(LifeGaugeKind::Max)) {
                i = static_cast<int32>(LifeGaugeKind::Min);
            }
            Game::Config().m_lifeGauge = static_cast<LifeGaugeKind>(i);
            SoundManager::PlaySe(U"desisionSmall");
            return true;
        }
        return false;
    }
    bool ChangeRandomNoteType()
    {
        if ((s3d::Key2 | s3d::KeyNum2).down()) {
            auto curt = Game::Config().m_random;
            int i = static_cast<int32>(curt);
            ++i;
            if (i > static_cast<int32>(RandomNoteType::Max)) {
                i = static_cast<int32>(RandomNoteType::Min);
            }
            Game::Config().m_random = static_cast<RandomNoteType>(i);
            SoundManager::PlaySe(U"desisionSmall");
            return true;
        }
        return false;
    }
    bool ChangePlayStyle()
    {
        if ((s3d::Key1 | s3d::KeyNum1).down()) {
            auto curt = Game::Config().m_styleType;
            PlayStyleType next = curt;
            switch (curt) {
            case ct::PlayStyleType::Normal:
                next = PlayStyleType::NormalArc;
                break;
            case ct::PlayStyleType::Portrait:
                next = PlayStyleType::Homography;
                break;
            case ct::PlayStyleType::NormalArc:
                next = PlayStyleType::Portrait;
                break;
            case ct::PlayStyleType::Homography:
                next = PlayStyleType::Landscape;
                break;
            case ct::PlayStyleType::Landscape:
                next = PlayStyleType::Normal;
                break;
            default:
                break;
            }
            Game::Config().m_styleType = next;
            SoundManager::PlaySe(U"desisionSmall");
            return true;
        }
        return false;
    }
}