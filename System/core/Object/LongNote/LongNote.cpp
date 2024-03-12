#include <core/Object/LongNote/LongNote.hpp>
#include <Useful.hpp>
#include <core/Judge/Judge.hpp>
#include <core/Play/PlayStyle/PlayStyle.hpp>
#include <core/Play/PlayMusicGame.hpp>
#include <core/Play/Random/RandomNote.hpp>
#include <utils/Addon/IntervalCounter.hpp>
#include <Siv3D.hpp>

namespace ct
{
    LongNote::LongNote(s3d::int64 timingSample, const NoteType type, double firstCount, double speed, std::shared_ptr<Note>& parent) :
        Object(timingSample, firstCount),
        m_baseType(type),
        m_type(type),
        m_scrollSpeed(speed),
        m_parent(parent)
    {
    }

    //
    void LongNote::perfect(s3d::int64 diff)
    {
        PlayMusicGame::ScoreUpdate(Score::Perfect, diff, m_parent->getType(), m_parent->getType(), false);
        m_isActive = false;
        m_parent->setActive(false);
    }

    void LongNote::miss(s3d::int64 diff)
    {
        PlayMusicGame::ScoreUpdate(Score::Miss, diff, m_parent->getType(), m_parent->getType(), false);
        m_isActive = false;
        m_parent->setActive(false);
    }

    void LongNote::init()
    {
        Object::init();

        // 配置決定
        m_type = m_parent->getType();

        // ロングの終点でロックを解除
        RandomNote::ResetIgnore();

        HSV hsv = m_parent->getColor();
        hsv.s = Min(0.5, hsv.s);
        hsv.v = 1.0;
        m_color = hsv.toColor();
        //判定処理
        switch (m_type) {
        case 11:
            m_judge = []() {
                return PlayKey::Red().pressed();
                };
            break;
        case 12:
            m_judge = []() {
                return PlayKey::Blue().pressed();
                };
            break;
        case 13:
            m_judge = []() {
                return PlayKey::Yellow().pressed();
                };
            break;
        case 14:
            m_judge = []() {
                return PlayKey::Blue().pressed() && PlayKey::Yellow().pressed();
                };
            break;
        case 15:
            m_judge = []() {
                return PlayKey::Red().pressed() && PlayKey::Yellow().pressed();
                };
            break;
        case 16:
            m_judge = []() {
                return PlayKey::Blue().pressed() && PlayKey::Red().pressed();
                };
            break;
        case 17:
            m_judge = []() {
                return PlayKey::Blue().pressed() && PlayKey::Red().pressed() && PlayKey::Yellow().pressed();
                };
            break;
        default:
            m_judge = [&]() {
                return false;
                };
        };
    }

    bool LongNote::update(const PlaybackState& state)
    {
        m_isActive = m_parent->isActive();//親のノーツの存在と同期

        if (!m_isActive || !m_parent->isFirstTap())
            return true;

        const auto timing = m_timingSample - state.samplePos;


        if (timing <= 0)//ロングの終点
        {
            this->perfect(timing);

            return true;
        }
        if (IntervalCounter::IsUpdatedEvery(3)) {
            if (m_type == 17) {
                // ↑なんでわけてるのか忘れた
                PlayStyle::Instance()->drawTapEffect(7, 17);
            } else {
                PlayStyle::Instance()->drawTapEffect(m_parent->getType(), m_parent->getType());
            }
        }
        //オートプレイ----------------------
        if (PlayContext::IsAutoPlay()) {
            return true;
        }
        //----------------------------------

        //親を押したのに途中で離した
        {
            if (!m_judge()) {
                int64 aTiming = s3d::Abs(timing);
                //パーフェクト範囲内での話はセーフ
                if (aTiming <= JudgeRange(Judge::Great))
                    this->perfect(timing);
                else
                    this->miss(timing);
            }
        }
        return true;
    }
    void LongNote::diffDraw(double count, double scrollRate)const
    {
        PlayStyle::Instance()->draw(*this, count, scrollRate);
    }
}
