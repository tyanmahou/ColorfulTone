#include "DownloadSceneView.hpp"

#include "DownloadScene.hpp"
#include "DownloadContent.hpp"
#include "SharedDraw.hpp"

class DownloadSceneView::Impl
{
private:
	const DownloadScene* const m_pScene;
	SharedDraw::DrawBGLight m_lights;
public:
	Impl(const DownloadScene* const scene):
		m_pScene(scene)
	{}

	void update()
	{
		m_lights.update();
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
				})
			.draw(
				contents,
				select.content,
				[](const DownloadContent & c)->decltype(auto) {return c.getTitle(); }
			);
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
