#pragma once 
#include <scenes/Scene/ISceneBase.hpp>
#include <core/Play/PlayMusicGame.hpp>
#include <core/Play/HighSpeed/HighSpeedDemo.hpp>

namespace ct
{
	class MainScene : public ISceneBase
	{
		enum class Mode
		{
			Free,
			Cource,
			Endless
		};
	public:
		MainScene(const InitData& init);
		~MainScene();

		void finally() override;

		void updateFadeIn(double t) override;
		void update() override;
		void draw() const override;
		void drawFadeIn(double t) const override;
		void drawFadeOut(double t) const override;

	private:
		PlayMusicGame m_musicGame;
		HighSpeedDemo m_highSpeed;

		std::shared_ptr<IPlaySession> m_lock;
	};
}
