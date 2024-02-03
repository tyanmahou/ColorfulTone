#include <core/Object/RepeatNote/RepeatNote.hpp>
#include <Useful.hpp>
#include <core/Judge/Judge.hpp>

#include <core/Play/PlayStyle/PlayStyle.hpp>
#include <core/Play/PlayMusicGame.hpp>
#include <Siv3D.hpp>

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
    bool RepeatNote::update(const PlayContext& context)
    {
        if (!m_isActive)
            return true;

        const auto timing = m_timingSample - context.samplePos;

        //判定範囲まで到達してなければタップ処理を行わない
        if (timing > JudgeRange(Judge::Good))
            return true;

        bool judge = PlayKey::Red().down() || PlayKey::Blue().down() || PlayKey::Yellow().down();

        if ((judge || timing <= 0) && m_isStart == false) {
            m_isStart = true;
        }
        return true;
    }
    void RepeatNote::diffDraw(double count, double scrollRate) const
    {
        PlayStyle::Instance()->draw(*this, count, scrollRate);

    }

    //---------------------------------------------------------------------
    s3d::int64 RepeatEnd::notesTapSample = 0;

    RepeatEnd::RepeatEnd(s3d::int64 timingSample, double firstCount, double speed, std::shared_ptr<Note>& parent, double interval) :
        LongNote(timingSample, 10, firstCount, speed, parent),
        m_interval(interval)
    {
    }

    void RepeatEnd::init()
    {
        m_isTap = false;
        m_isStart = false;
        LongNote::init();
        //どれかおしたら
        m_judge = []() {
            return PlayKey::Red().down() || PlayKey::Blue().down() || PlayKey::Yellow().down();
        };
    }

    bool RepeatEnd::update(const PlayContext& context)
    {
        if (!m_isActive || !m_parent->isFirstTap())
            return true;

        const auto samplePos = context.samplePos;
        const auto timing = m_timingSample - samplePos;

        // 1小節のSample
        const double timePerBar = 60.0 * 4.0 / context.bpm;
        const s3d::int64 samplePerBar = static_cast<s3d::int64>(timePerBar * 44100);

        //初期化
        if (!m_isStart) {
            m_isStart = true;
            m_lastSamplePos = samplePos;
        }
        //オートプレイ----------------------
        if (AutoPlayManager::IsAutoPlay()) {
            if (m_lastSamplePos == samplePos || samplePos > m_lastSamplePos + samplePerBar / (m_interval * 2)) {
                static int tap = 0;
                ++tap %= 3;
                AutoPlayManager::Input(tap + 1);
                EffectUpdate(tap == 0, tap == 1, tap == 2);
                m_lastSamplePos = samplePos;
                m_isTap = true;
            }
        } else
            //----------------------------------
            if (m_judge()) {
                EffectUpdate(PlayKey::Red().down(), PlayKey::Blue().down(), PlayKey::Yellow().down());
                m_lastSamplePos = samplePos;
                m_isTap = true;
            }

        if (notesTapSample > m_lastSamplePos) {
            m_lastSamplePos = notesTapSample;
            m_isTap = true;
        }


        if (timing <= 0)//ロングの終点
        {
            if (m_isTap)
                this->perfect();
            else
                this->miss();

            return false;
        }
        //8分間隔以内で連打しないとミス
        if (samplePos > m_lastSamplePos + samplePerBar / m_interval + JudgeRange(Judge::Perfect)) {
            int64 aTiming = s3d::Abs(timing);
            //パーフェクト範囲内での話はセーフ
            if (aTiming <= JudgeRange(Judge::Perfect) && m_isTap)
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
