#include <core/Object/Text/TextObjcet.hpp>
#include <core/Play/PlayMusicGame.hpp>
#include <core/SoundBeat.hpp>
#include <Siv3D/PutText.hpp>
#include <Siv3D/Scene.hpp>

namespace ct
{
    bool TextObject::update(double nowCount, double countPerFrame)
    {
        const Audio* const pSound = PlayMusicGame::CurrentSound();

        if (!m_isActive || !pSound)
            return true;
        const auto sample = GetSamplePos(*pSound);
        auto count = m_count - nowCount;
        if (s3d::Abs(count) < s3d::Abs(count - countPerFrame))
            count = 0;

        //ストップ時にオフセットの変更

        //停止初期化
        if (!m_isDraw && count <= 0) {
            m_isDraw = true;
            m_beginTimeSample = sample;
        }
        if (m_isDraw) {
            if (static_cast<s3d::uint64>(sample) >= m_beginTimeSample + m_drawTimeSample) {
                m_isDraw = false;
                m_isActive = false;
            }
        }
        return true;
    }
    void TextObject::diffDraw([[maybe_unused]]double count, [[maybe_unused]] float scrollRate)const
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
