#include "DownloadSceneView.hpp"

#include"DownloadScene.hpp"

class DownloadSceneView::Impl
{
private:
	const DownloadScene* const m_pScene;
public:
	Impl(const DownloadScene* const scene):
		m_pScene(scene)
	{}

	void draw()const
	{

	}
};

DownloadSceneView::DownloadSceneView(const DownloadScene* const scene):
	m_pImpl(std::make_shared<Impl>(scene))
{
}

void DownloadSceneView::draw() const
{
	m_pImpl->draw();
}
