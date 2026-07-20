#pragma once
#include <memory>

namespace ct
{
	class PlaylistScene;

	class PlaylistSceneView
	{
	private:
		class Impl;
		std::shared_ptr<Impl> m_pImpl;
	public:
		PlaylistSceneView(const PlaylistScene* const scene);

		~PlaylistSceneView() = default;

		void update() const;
		void draw()const;
	};
}