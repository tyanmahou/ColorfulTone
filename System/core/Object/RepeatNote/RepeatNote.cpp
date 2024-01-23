#include <core/Object/RepeatNote/RepeatNote.hpp>
#include <Useful.hpp>
#include <core/Judge/Judge.hpp>
#include <core/Input/AutoPlayManager.hpp>
#include <core/Input/PlayKey.hpp>

#include <core/Play/PlayStyle/PlayStyle.hpp>
#include <core/Play/PlayMusicGame.hpp>

namespace
{
    using namespace ct;
    void EffectUpdate(bool red, bool blue, bool yellow)
    {
        if (red)
            PlayStyle::Instance()->drawTapEffect(1);
        if (blue)
            PlayStyle::Instance()->drawTapEffect(2);
        if (yellow)
            PlayStyle::Instance()->drawTapEffect(3);
        SoundManager::PlayInGameSe(U"PERFECT");
    }
}

namespace ct
{
    void RepeatNote::init()
    {
        m_isStart = false;
        Note::init();
    }
    bool RepeatNote::update(double nowCount, double countPerFrame)
    {
        if (!m_isActive)
            return true;

        const double count = m_count - nowCount;

        //判定範囲まで到達してなければタップ処理を行わない
        if (count > JudgeRange(countPerFrame, Judge::Good))
            return true;

        bool judge = PlayKey::Red().down() || PlayKey::Blue().down() || PlayKey::Yellow().down();

        if ((judge || count <= 0) && m_isStart == false) {
            m_isStart = true;
        }
        return true;
    }
    void RepeatNote::diffDraw(double count, double scrollRate) const
    {
        PlayStyle::Instance()->draw(*this, count, scrollRate);

    }

    //---------------------------------------------------------------------
    double RepeatEnd::notesTapCount = 0;

    RepeatEnd::RepeatEnd(double firstCount, double speed, std::shared_ptr<Note>& parent, double interval) :
        LongNote(10, firstCount, speed, parent),
        m_interval(interval)
    {
        //どれかおしたら
        m_judge = []() {
            return PlayKey::Red().down() || PlayKey::Blue().down() || PlayKey::Yellow().down();
            };
    }

    void RepeatEnd::init()
    {
        m_isTap = false;
        m_isStart = false;
        LongNote::init();
    }

    bool RepeatEnd::update(double nowCount, double countPerFrame)
    {
        if (!m_isActive || !m_parent->isFirstTap())
            return true;

        auto count = m_count - nowCount;

        //初期化
        if (!m_isStart) {
            m_isStart = true;
            m_lastCount = nowCount;
        }
        //オートプレイ----------------------
        if (AutoPlayManager::IsAutoPlay()) {
            if (m_lastCount == nowCount || nowCount > m_lastCount + NotesData::RESOLUTION / (m_interval * 2)) {
                static int tap = 0;
                ++tap %= 3;
                AutoPlayManager::Input(tap + 1);
                EffectUpdate(tap == 0, tap == 1, tap == 2);
                m_lastCount = nowCount;
                m_isTap = true;
            }
        } else
            //----------------------------------
            if (m_judge()) {
                EffectUpdate(PlayKey::Red().down(), PlayKey::Blue().down(), PlayKey::Yellow().down());
                m_lastCount = nowCount;
                m_isTap = true;
            }

        if (notesTapCount > m_lastCount) {
            m_lastCount = notesTapCount;
            m_isTap = true;
        }


        if (count <= 0)//ロングの終点
        {
            if (m_isTap)
                this->perfect();
            else
                this->miss();

            return false;
        }
        //8分間隔以内で連打しないとミス
        if (nowCount > m_lastCount + NotesData::RESOLUTION / m_interval) {
            auto aCount = s3d::Abs(count);
            //パーフェクト範囲内での話はセーフ
            if (aCount <= JudgeRange(countPerFrame, Judge::Perfect) && m_isTap)
                this->perfect();
            else
                this->miss();
        }
        return false;
    }

    void RepeatEnd::diffDraw(double count, double scrollRate) const
    {
        PlayStyle::Instance()->draw(*this, count, scrollRate);
    }
}
