#include <scenes/utils/SharedDraw/LoadingProgress.hpp>
#include <commons/FontName.hpp>
#include <Siv3D.hpp>

namespace ct::SharedDraw
{

    LoadingProgress::LoadingProgress(const s3d::String& text) :
        m_message(text),
        m_stopwatch(StartImmediately::Yes),
        m_isCompleted(false)
    {
    }
    void LoadingProgress::onCompleted()
    {
        m_isCompleted = true;
        m_stopwatch.restart();
        m_message = U"COMPLETED";
    }
    void LoadingProgress::update()
    {
        // now loadingのインターバル時間
        constexpr int nowLoadingInterval = 4;
        if (m_stopwatch.s() > nowLoadingInterval) {
            m_stopwatch.restart();
        }
    }
    void LoadingProgress::draw(double progress) const
    {
        const Vec2 center = Scene::Center();
        const int32 timer = static_cast<int32>(Scene::Time() * 60) % 360;

        const double rad = static_cast<double>(timer) / 360 * Math::TwoPi;
        const double startAngle = rad;
        const double mod = Fmod(rad, 1.0);
        const double angle = Math::TwoPi * Pow(2.0 * (mod > 1.0 ? 1.0 : mod) - 1.0, 3);

        if (m_isCompleted) {
            const double size = static_cast<double>(Min(m_stopwatch.ms(), 350));
            Line(center + Vec2{ size, 20 }, center + Vec2{ -size,20 }).draw(ColorF(0, 0.5));
            Circle(center, 200 + m_stopwatch.ms()).drawFrame(0, 1.2, ColorF(0, 0.5));
        } else {
            Circle(center, 200).drawArc(startAngle, angle, 0, 1.2, ColorF(0, 0.5));
        }
        const double index = m_stopwatch.ms() / (m_isCompleted ? 50 : 160.0);
        //現在の文
        FontAsset font(FontName::LodingLabel);
        const auto offset = font(m_message).region().size / 2.0;
        Vec2 topLeft = center - offset;
        Vec2 penPos{ topLeft };
        for (const auto& [ i, glyph] : s3d::Indexed(font.getGlyphs(m_message))) {
            const double t = index - i;
            // 改行文字なら
            if (glyph.codePoint == U'\n') {
                // ペンの X 座標をリセット
                penPos.x = topLeft.x;

                // ペンの Y 座標をフォントの高さ分進める
                penPos.y += font.height();

                continue;
            }

            if (glyph.codePoint != U' ') {
                ColorF color(0.3, 1);
                color.a = static_cast<uint32_t>(Saturate(t / 6) * 255);

                Vec2 drawPos = penPos + glyph.getOffset();
                drawPos.y -= EaseInQuad(Saturate(1 - t / 4)) * 20.0;
                glyph.texture.draw(Math::Round(drawPos), color);
            }

            penPos.x += glyph.xAdvance;
        }

        // bar
        RectF(0, 570, (800 + 20) * progress, 30).draw(Color(0, 0, 0, 128));
    }
    s3d::int32 LoadingProgress::getStopwatchMs() const
    {
        return m_stopwatch.ms();
    }
}
