#include <scenes/Scene/FileLoad/FileLoadSceneView.hpp>
#include <scenes/Scene/FileLoad/FileLoadScene.hpp>
#include <scenes/utils/SharedDraw.hpp>
#include <Siv3D.hpp>

namespace ct
{
	class FileLoadSceneView::Impl
	{
	private:
		SharedDraw::DrawBGLight m_lights;
		SharedDraw::LoadingProgress m_loading;
		const FileLoadScene* m_scene;
	public:
		Impl(const FileLoadScene* const scene):
			m_scene(scene)
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
			m_loading.draw(m_scene->progress());
		}

		void onCompleted()
		{
			m_loading.onCompleted();
		}
		int32 getStopwatchMs() const
		{
			return m_loading.getStopwatchMs();
		}
	};
	FileLoadSceneView::FileLoadSceneView(const FileLoadScene* const scene) :
		m_pImpl(std::make_shared<Impl>(scene))
	{}

	FileLoadSceneView::~FileLoadSceneView()
	{}

	void FileLoadSceneView::update()
	{
		m_pImpl->update();
	}
	void FileLoadSceneView::draw() const
	{
		m_pImpl->draw();
	}

	void FileLoadSceneView::onCompleted()
	{
		m_pImpl->onCompleted();
	}
}