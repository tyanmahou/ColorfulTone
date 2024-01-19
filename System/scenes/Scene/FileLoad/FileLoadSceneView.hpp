#pragma once
#include <Siv3D/Fwd.hpp>
#include <memory>

namespace ct
{
	class FileLoadScene;
	// ロード画面ビュー
	class FileLoadSceneView
	{
	private:
		class Impl;
		std::shared_ptr<Impl> m_pImpl;
	public:
		FileLoadSceneView(const FileLoadScene* const scene);
		~FileLoadSceneView();
		void update();
		void draw()const;
		void onCompleted();
	};
}
