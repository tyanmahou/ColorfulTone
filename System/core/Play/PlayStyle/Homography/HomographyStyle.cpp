#include <core/Play/PlayStyle/Homography/HomographyStyle.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace
{
    using namespace ct;
    constexpr double g_width = 252;
    constexpr double homographyOffset = 600;
}

void HomographyStyle::drawFrame(bool red, bool blue, bool yellow, std::function<void()> drawCallback) const
{
    const auto& config = Game::Config();

    {
        auto scoped = Shaders::Gizi3D().start();
        {
            ScopedRenderStates2D blend(BlendState::Subtractive);
            const double w = g_width * config.m_playScale;
            RectF(400 - w / 2, -homographyOffset, w, 600 + homographyOffset).draw(ColorF(0.3, 0.6));
        }
        {
            Transformer2D t2d(Mat3x2::Scale(config.m_playScale, Vec2{ 400, 500 }));

            constexpr double height = 400;
            if (red) {
                constexpr ColorF color[4] = { ColorF(1,0,0,0),ColorF(1,0,0,0),Palette::White,Palette::White };
                RectF(400 - g_width / 2, 500 - height, g_width / 3, height).draw(color);
            }
            if (blue) {
                constexpr ColorF color[4] = { ColorF(0,0,1,0), ColorF(0,0,1,0),Palette::White,Palette::White };
                RectF(400 - g_width / 6, 500 - height, g_width / 3, height).draw(color);
            }
            if (yellow) {
                constexpr ColorF color[4] = { ColorF(1,1,0,0),ColorF(1,1,0,0),Palette::White,Palette::White };
                RectF(400 + g_width / 6, 500 - height, g_width / 3, height).draw(color);
            }

            this->drawJudgeLine();

            m_effetcs[0].update();
            drawCallback();
            {
                ScopedRenderStates2D blend(BlendState::Additive);
                m_effetcs[1].update();
            }
        }
    }
}

bool HomographyStyle::canDraw(double y) const
{
    const double scale = Game::Config().m_playScale;
    constexpr double topBase = -homographyOffset - 50;
    const double top = 500 - (500 - topBase) / scale;

    constexpr double bottomBase = 650;
    const double bottom = 500 - (500 - bottomBase) / scale;

    return (top <= y && y <= bottom);
}
