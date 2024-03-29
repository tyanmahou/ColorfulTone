#include <core/Anime/ReadyAnime/ReadyAnime.hpp>
#include <commons/FontName.hpp>
#include <Siv3D.hpp>

namespace ct
{
    ReadyAnime::ReadyAnime()
    {
    }
    ReadyAnime::~ReadyAnime()
    {
    }
    void ReadyAnime::playIn()
    {
        m_fadeTimer.reset(0.3);
        m_isPlayIn = true;
    }
    void ReadyAnime::playOut()
    {
        m_fadeTimer.reset(0.3);
        m_isPlayIn = false;
    }
    bool ReadyAnime::isPlaying() const
    {
        return m_fadeTimer.isRunning();
    }
    bool ReadyAnime::isEnd() const
    {
        return m_fadeTimer.isEnd();
    }
    void ReadyAnime::update(double dt)
    {
        m_fadeTimer.update(dt);
    }
    void ReadyAnime::draw() const
    {
        if (!m_isPlayIn && !m_fadeTimer.isRunning()) {
            return;
        }
        if (m_isPlayIn) {
            double r = m_fadeTimer.rate();
            r = 1 - (1 - r) * (1 - r) * (1 - r);
            TextureAsset(U"ready_label").scaled(r, 1.0).drawAt(Scene::CenterF());
            int32 i = -2;
            for (auto ch : U"READY") {
                Vec2 pos = Scene::CenterF();
                pos.x += i * 70 * r;
                FontAsset(FontName::MainReady)(ch).drawAt(pos, Palette::White);
                ++i;
            }
        } else {
            double r = m_fadeTimer.invRate();
            TextureAsset(U"ready_label").scaled(1.0, r).drawAt(Scene::CenterF(), ColorF(1, r));
            int32 i = -2;
            for (auto ch : U"READY") {
                Vec2 pos = Scene::CenterF();
                pos.x += i * 70;
                FontAsset(FontName::MainReady)(ch).drawAt(pos, ColorF(1, r));
                ++i;
            }

        }
    }
}
