#include "DownloadSceneView.hpp"

#include "DownloadScene.hpp"
#include "DownloadContent.hpp"
#include "SharedDraw.hpp"

namespace {
	void DrawJacket(const DownloadContent* pContent)
	{
		if (!pContent) {
			return;
		}
		// ジャケ絵描画
		const Vec2 pos{ Constants::JacketCenter, 180 };
		const Vec2 size{ 210, 210 };
		RectF(pos - size / 2.0, size).draw();
		pContent
			->getTexture()
			.resize(size)
			.rotate(Math::Radians(-7.0))
			.drawAt(pos, pContent->getColor());
	}
}
class DownloadSceneView::Impl
{
private:
	const DownloadScene* const m_pScene;
	SharedDraw::DrawBGLight m_lights;
	SharedDraw::LoadingProgress m_loading;
public:
	Impl(const DownloadScene* const scene):
		m_pScene(scene),
		m_loading(L"DOWNLOADING...")
	{}

	void update()
	{
		m_lights.update();
		m_loading.update();
	}

	void draw()const
	{
		TextureAsset(L"canvasBg").draw();
		m_lights.draw();

		const auto select = DownloadScene::GetSelectInfo();
		const auto& contents = m_pScene->getContents();
		const DownloadContent* pContent = contents.size() 
			? &contents[select.content] : nullptr;

		const int moveSelect = m_pScene->getMoveSelect();
		static EasingController<double> easingAnime(0.0, -30.0, Easing::Linear, 100);
		if (moveSelect)
		{
			easingAnime.reset();
			easingAnime.start();
		}
		const double offset = easingAnime.isActive() ?
			easingAnime.easeInOut() :
			easingAnime.getEnd();

		SharedDraw::Select<DownloadContent>()
			.setOffset(offset)
			.setColorCallBack([](const DownloadContent & c) {
					if (c.isDownloaded())return Palette::Gray;
					return Palette::White;
				}
			)
			.draw(
				contents,
				select.content,
				[](const DownloadContent & c)->decltype(auto) {
					return c.getTitle(); 
				}
			);

		if (pContent) {
			static SharedDraw::JacketInfo info;
			info
				.setPos({ Constants::JacketCenter, 320 })
				.drawLine()
				.drawSub(pContent->getTitle());

			::DrawJacket(pContent);
			TextureAsset(L"memoDL").drawAt(Constants::JacketCenter, 450);
			FontAsset(L"info")(pContent->getDetail()).draw({ 90, 380 },Palette::Black, 1.5);
		}


		static const String title = L"DOWNLOAD";
		SharedDraw::Sticky(&title, nullptr);

		if (m_pScene->isDownloading())
		{
			Window::BaseClientRect().draw(ColorF(1, 0.5));
			m_loading.draw(m_pScene->getProgress());
		}
	}
};

DownloadSceneView::DownloadSceneView(const DownloadScene* const scene):
	m_pImpl(std::make_shared<Impl>(scene))
{
}

void DownloadSceneView::update() const
{
	m_pImpl->update();
}

void DownloadSceneView::draw() const
{
	m_pImpl->draw();
}
