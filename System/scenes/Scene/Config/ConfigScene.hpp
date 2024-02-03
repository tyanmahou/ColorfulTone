#pragma once
#include <scenes/Scene/ISceneBase.hpp>
#include <scenes/Scene/Config/ConfigMain.hpp>

namespace ct
{
	class ConfigScene :public ISceneBase
	{
	public:

		ConfigScene(const InitData& init);
		~ConfigScene();

		void update() override;
		void draw() const override;

		void drawFadeIn(double t) const override;
		void drawFadeOut(double t) const override;
	private:
		ConfigMain m_config;
	};
}
