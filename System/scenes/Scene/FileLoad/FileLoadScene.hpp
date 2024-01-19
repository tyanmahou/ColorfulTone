#pragma once
#include <scenes/Scene/ISceneBase.hpp>
#include <scenes/Scene/FileLoad/FileLoadSceneView.hpp>
#include <utils/Coro/Fiber/FiberHolder.hpp>

namespace ct
{
	class FileLoadScene : public ISceneBase
	{
	private:
		enum class State
		{
			Loading,
			LoadCompleted,
			Wait
		};
	public:

		FileLoadScene(const InitData& init);
		~FileLoadScene();

		void update() override;
		void draw() const override;
		void drawFadeIn(double t) const override;
		void drawFadeOut(double t) const override;

		double progress() const;
	private:
		Coro::Fiber<void> updateAsync();
	private:
		State m_state = State::Loading;
		FileLoadSceneView m_view;	//ビュー
		Coro::FiberHolder<void> m_asyncUpdater;
	};
}
