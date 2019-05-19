#include "TitleSceneView.hpp"

#include "TitleScene.h"
#include"SharedDraw.hpp"
#include"FontKinetic.h"

namespace
{
	void DrawBgs(double timer)
	{
		static RenderTexture tex(700, 500);
		{
			auto render = util::RenderTextureUtil(tex);
			auto t2d = render.getTransformer2D();
			TextureAsset(L"titleBgs").uv(timer / 15000.0, 0.0, 0.75, 1.0 / 3.0).draw(0, 0);
			TextureAsset(L"titleBgs").uv(timer / 6000.0, 1.0 / 3.0, 0.75, 1.0 / 3.0).draw(0, 0);
			TextureAsset(L"titleBgs").uv(timer / 2000.0, 2.0 / 3.0, 0.75, 1.0 / 3.0).draw(0, 0);
		}
		Graphics2D::SetBlendState(BlendState::Multiplicative);
		tex.draw(50, 60);
		Graphics2D::SetBlendState(BlendState::Default);
	}
}
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
		TextureAsset(L"canvasBg").draw();
		static SharedDraw::DrawBGLight lights;
		lights.update();
		lights.draw();
		const double timer = static_cast<double>(System::FrameCount());

		::DrawBgs(timer);
		TextureAsset(L"logo").scale(0.8).drawAt(400, 150);

		static Array<String> name = {
			L"FREE PLAY",
			L"COURSE PLAY",
			L"CONFIG",
			L"DOWNLOAD",
			L"RELOAD",
			L"TUTORIAL",
			L"ACCESS",
			L"EXIT",
		};

		constexpr double x = 400;
		constexpr double y = 350;
		constexpr int off = 40;

		TextureAsset(L"modeBack").drawAt(x, y + 40 * 2, ColorF(0.8, 1, 1, 0.9 + 0.1 * Sin(timer / 30.0)));

		auto mode = TitleScene::GetMode();
		auto font = FontAsset(L"10o_s");
		Graphics2D::SetSamplerState(SamplerState::ClampLinear);
		for (unsigned int i = 0; i < 5; ++i)
		{
			const unsigned index = (static_cast<unsigned int>(mode) + i + 6) % 8;
			const auto size = font(name[index]).region().size;
			font(name[index]).drawKinetic(x - size.x / 2.0, y + off * i - size.y / 2.0, FontKinetic::DeleteSpace, ColorF(0, i == 2 ? 1 : 0.5 - 0.1 * Abs<int>(2 - i)));

			const Vec2 xy{ index % 4 * 250,index / 4 * 250 };
			constexpr Vec2 texSize{ 250,250 };
			TextureAsset(L"titleIcons")(xy, texSize)
				.scale(0.15 + (i == 2 ? 0.012 * Sin(timer / 20.0) : 0))
				.drawAt(x - 150, y + off * i, ColorF(i == 2, 1 - 0.3 * Abs<int>(2 - i)));
		}
		Graphics2D::SetSamplerState(SamplerState::Default2D);
	}
};

TitleSceneView::TitleSceneView(const TitleScene * const scene):
	m_pImpl(std::make_shared<Impl>(scene))
{}

void TitleSceneView::draw() const
{
	m_pImpl->draw();
}
