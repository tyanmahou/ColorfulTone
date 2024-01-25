#include <scenes/Scene/Download/DownloadSceneView.hpp>
#include <scenes/Scene/Download/DownloadScene.hpp>
#include <core/Data/DownloadContent/DownloadContent.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace {
	using namespace ct;

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
			.resized(size)
			.rotated(Math::ToRadians(-7.0))
			.drawAt(pos, pContent->getColor());
	}
}

namespace ct
{

	class DownloadSceneView::Impl
	{
	private:
		const DownloadScene* const m_pScene;
		SharedDraw::DrawBGLight m_lights;
		SharedDraw::LoadingProgress m_loading;
	public:
		Impl(const DownloadScene* const scene) :
			m_pScene(scene),
			m_loading(U"DOWNLOADING...")
		{}

		void update()
		{
			m_lights.update();
			m_loading.update();
		}

		void draw()const
		{
			TextureAsset(U"canvasBg").draw();
			m_lights.draw();

			const auto select = DownloadScene::GetSelectInfo();
			const auto& contents = m_pScene->getContents();
			const DownloadContent* pContent = contents.size()
				? &contents[select.content] : nullptr;

			const int moveSelect = m_pScene->getMoveSelect();
			static EasingAB<double> easingAnime(0.0, -30.0, Easing::Linear, 100);
			if (moveSelect) {
				easingAnime.reset();
				easingAnime.start();
			}
			const double offset = easingAnime.isMoving() ?
				easingAnime.easeInOut() :
				easingAnime.done();

			SharedDraw::Select<DownloadContent>()
				.setOffset(offset)
				.setColorCallBack([](const DownloadContent& c) { 
				    return c.isDownloaded() ? Palette::Gray : Palette::White;
				})
				.setDrawble([](const DownloadContent& c, Vec2 pos) {
					c.getTexture().resized(50, 50).drawAt(pos + Vec2{ 37, 30 }, c.getColor());
				})
				.draw(
					contents,
					select.content,
					[](const DownloadContent& c)->decltype(auto) {
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
				TextureAsset(U"memoDL").drawAt(Constants::JacketCenter, 450);
				FontKinetic::LineSpacing(FontAsset(FontName::Info), pContent->getDetail(), Vec2{ 90, 380 }, Palette::Black, 1.3);
			}


			static const String title = U"DOWNLOAD";
			SharedDraw::Sticky(&title, nullptr);
			if (m_pScene->isListLoading()) {
				Scene::Rect().draw(ColorF(1, 0.5));
			} else if (m_pScene->isDownloading()) {
				Scene::Rect().draw(ColorF(1, 0.5));
				m_loading.draw(m_pScene->getProgress());
			}
		}
	};

	DownloadSceneView::DownloadSceneView(const DownloadScene* const scene) :
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

}