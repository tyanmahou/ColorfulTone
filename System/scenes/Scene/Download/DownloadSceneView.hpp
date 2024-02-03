#pragma once
#include <memory>

namespace ct
{
	class DownloadScene;

	class DownloadSceneView
	{
	private:
		class Impl;
		std::shared_ptr<Impl> m_pImpl;
	public:
		DownloadSceneView(const DownloadScene* const scene);
		~DownloadSceneView() = default;

		void update() const;
		void draw()const;
	};
}
