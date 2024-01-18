#pragma once
#include <scenes/Scene/ISceneBase.hpp>
#include <scenes/Scene/FileLoad/FileLoadSceneView.hpp>

namespace ct
{
	class FileLoadScene : public ISceneBase
	{
	private:
		enum class State
		{
			Loading,
			OnLoadCompleted,
			Wait
		} m_state = State::Loading;
		FileLoadSceneView m_view;	//ビュー
	public:

		FileLoadScene(const InitData& init);
		~FileLoadScene();

		void update() override;
		void draw() const override;
		void drawFadeIn(double t) const override;
		void drawFadeOut(double t) const override;

		double progress() const;
	};
}
