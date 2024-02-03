#include <scenes/Scene/Title/TitleSceneView.hpp>
#include <scenes/Scene/Title/TitleScene.hpp>
#include <scenes/utils/ScopedResetTransRT.hpp>
#include <commons/FontName.hpp>
#include <scenes/utils/SharedDraw.hpp>
#include <scenes/utils/Font/FontKinetic.hpp>
#include <Siv3D.hpp>

namespace
{
    void DrawBgs(double timer)
    {
        static RenderTexture tex(700, 500);
        {
            auto render = ct::ScopedResetTransRT(tex);
            tex.clear(Palette::White);
            TextureAsset(U"titleBgs").uv(timer / 15000.0, 0.0, 0.75, 1.0 / 3.0).draw(0, 0);
            TextureAsset(U"titleBgs").uv(timer / 6000.0, 1.0 / 3.0, 0.75, 1.0 / 3.0).draw(0, 0);
            TextureAsset(U"titleBgs").uv(timer / 2000.0, 2.0 / 3.0, 0.75, 1.0 / 3.0).draw(0, 0);
        }
        {
            ScopedRenderStates2D scoped(BlendState::Multiplicative);
            tex.draw(50, 60);
        }
    }
}

namespace ct
{
    class TitleSceneView::Impl
    {
    private:
        const TitleScene* const m_pScene;
    public:
        Impl(const TitleScene* const scene) :
            m_pScene(scene)
        {}
        void draw()const
        {
            TextureAsset(U"canvasBg").draw();
            static SharedDraw::DrawBGLight lights;
            lights.update();
            lights.draw();
            const double timer = Scene::Time() * 60.0;

            ::DrawBgs(timer);
            TextureAsset(U"logo").scaled(0.8).drawAt(400, 150);

            static constexpr StringView name[] = {
                U"FREE PLAY",
                U"COURSE PLAY",
                U"CONFIG",
                U"DOWNLOAD",
                U"RELOAD",
                U"TUTORIAL",
                U"ACCESS",
                U"LICENCE",
                U"EXIT",
            };
            static constexpr size_t modeCount = sizeof(name) / sizeof(StringView);
            constexpr double x = 400;
            constexpr double y = 350;
            constexpr int off = 40;

            TextureAsset(U"modeBack").drawAt(x, y + 40 * 2, ColorF(0.8, 1, 1, 0.9 + 0.1 * Sin(timer / 30.0)));

            auto mode = TitleScene::GetMode();
            auto font = FontAsset(FontName::TitleLabel);
            {
                ScopedRenderStates2D sampler(SamplerState::ClampLinear);
                for (size_t i = 0; i < 5; ++i) {
                    const size_t index = (static_cast<size_t>(mode) + i + (modeCount - 2)) % modeCount;
                    const auto size = font(name[index]).region().size;

                    int diff = s3d::Abs(static_cast<int>(2 - i));
                    const ColorF color(0, i == 2 ? 1 : 0.5 - 0.1 * diff);
                    Vec2 pos{ x - size.x / 2.0, y + off * i - size.y / 2.0 };
                    auto outline = TextStyle::Outline(0.3, color);
                    FontKinetic::DeleteSpace(font, name[index], outline, pos, color);

                    const Vec2 xy{ index % 4 * 250,index / 4 * 250 };
                    constexpr Vec2 texSize{ 250,250 };
                    TextureAsset(U"titleIcons")(xy, texSize)
                        .scaled(0.15 + (i == 2 ? 0.012 * Sin(timer / 20.0) : 0))
                        .drawAt(x - 150, y + off * i, ColorF(i == 2, 1 - 0.3 * diff));
                }
            }
        }
    };

    TitleSceneView::TitleSceneView(const TitleScene* const scene) :
        m_pImpl(std::make_unique<Impl>(scene))
    {}
    TitleSceneView::~TitleSceneView()
    {}

    void TitleSceneView::draw() const
    {
        m_pImpl->draw();
    }

}