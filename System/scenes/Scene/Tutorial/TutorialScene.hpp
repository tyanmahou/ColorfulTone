#pragma once
#include <scenes/Scene/ISceneBase.hpp>
#include <core/Play/PlayMusicGame.hpp>
#include <core/Play/HighSpeed/HighSpeedDemo.hpp>
#include <core/Data/MusicData/MusicData.hpp>

namespace ct
{
	class TutorialScene : public ISceneBase
	{
	public:
		TutorialScene(const InitData& init);
		~TutorialScene();

		void finally() override;

		void updateFadeIn(double t) override;
		void update() override;
		void draw() const override;
		void drawFadeIn(double t) const override;
		void drawFadeOut(double t) const override;

	private:
		bool m_isCourse = false;

		PlayMusicGame m_musicGame;
		HighSpeedDemo m_highSpeed;

		MusicData m_music;
	};
}
