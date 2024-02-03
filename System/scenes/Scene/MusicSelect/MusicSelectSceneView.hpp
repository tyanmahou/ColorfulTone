#pragma once
#include <memory>

namespace ct
{
	class MusicSelectScene;

	class MusicSelectSceneView
	{
		class Impl;
		std::shared_ptr<Impl> m_pImpl;
	public:
		MusicSelectSceneView(const MusicSelectScene* const scene);
		~MusicSelectSceneView();

		void resetShaderTimer();
		void onChangeAction();
		void update();

		void draw()const;
	};

}
