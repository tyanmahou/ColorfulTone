#include <scenes/Scene/Config/ConfigScene.hpp>
#include <Useful.hpp>

namespace ct
{
	//-----------------------------------------------------------------------------------------

	ConfigScene::ConfigScene(const InitData& init):
		ISceneBase(init)
	{
	}

	ConfigScene::~ConfigScene()
	{}


	//--------------------------------------------------------------------------------
	//関数：update
	//--------------------------------------------------------------------------------


	void ConfigScene::update()
	{
		m_config.update();

		if (PlayKey::BigBack().down()) {
			SoundManager::PlaySe(U"desisionLarge");
			changeScene(SceneName::Title, 1000);
		}
	}

	//--------------------------------------------------------------------------------
	//関数：draw
	//--------------------------------------------------------------------------------

	void ConfigScene::draw()const
	{
		TextureAsset(U"canvasBg").draw();
		static SharedDraw::DrawBGLight lights;
		lights.update();
		lights.draw();

		static const String title = U"CONFIG";
		SharedDraw::Sticky(&title, nullptr);

		m_config.drawWithBack();

		SceneInfo::DrawEsc();
		SceneInfo::Draw(U"[Enter]決定 [BackSpace]戻る");
	}

	//--------------------------------------------------------------------------------
	//関数：drawFadeIn
	//--------------------------------------------------------------------------------
	void ConfigScene::drawFadeIn(double t) const
	{
		FadeIn(Fade::FlipPage, t, [this]() {draw(); }, true);
	}

	//--------------------------------------------------------------------------------
	//関数：drawFadeOut
	//--------------------------------------------------------------------------------
	void ConfigScene::drawFadeOut([[maybe_unused]]double t) const
	{
		this->draw();
	}
}
