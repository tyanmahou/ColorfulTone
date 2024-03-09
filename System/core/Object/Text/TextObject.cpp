#include <core/Object/Text/TextObject.hpp>
#include <core/Play/PlayMusicGame.hpp>
#include <core/SoundBeat.hpp>
#include <Siv3D/PutText.hpp>
#include <Siv3D/Scene.hpp>
#include <core/Judge/Judge.hpp>

namespace ct
{
    bool TextObject::update(const PlaybackState& state)
    {
        if (!m_isActive)
            return true;
        const auto samplePos = state.samplePos;
        auto timing = m_timingSample - samplePos;
        if (s3d::Abs(timing) < s3d::Abs(timing - OneFrameSample()))
            timing = 0;

        //ストップ時にオフセットの変更

        //停止初期化
        if (!m_isDraw && timing <= 0) {
            m_isDraw = true;
            m_beginTimeSample = samplePos;
        }
        if (m_isDraw) {
            if (static_cast<s3d::uint64>(samplePos) >= m_beginTimeSample + m_drawTimeSample) {
                m_isDraw = false;
                m_isActive = false;
            }
        }
        return true;
    }
    void TextObject::diffDraw([[maybe_unused]]double count, [[maybe_unused]] double scrollRate)const
    {
        if (m_isDraw) {
            const auto win = s3d::Scene::Size();
            s3d::PutText(
                m_msg,
                static_cast<double>(win.x) / 2.0,
                static_cast<double>(win.y) / 3.0
            );
        }
    }

    void TextObject::init()
    {
        Object::init();
        m_isDraw = false;
    }
}
