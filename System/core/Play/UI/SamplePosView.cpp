#include <core/Play/UI/SamplePosView.hpp>
#include <Siv3D.hpp>

namespace ct
{
    void SamplePosView::draw() const
    {
        Transformer2D t2d(Mat3x2::Translate(m_xOffs, 0));

        constexpr s3d::int32 barY = 50;
        TextureAsset(U"streamPosBase").draw(-5, barY);

        constexpr auto size = (barY + 454 - 10) - (barY + 11);
        const Line bar({ -5 + 13.5, barY + 11 + size * m_rate }, { -5 + 13.5, barY + 11 + 10 + size * m_rate });
        
        bar.draw(3, Palette::Orange);
    }
}
