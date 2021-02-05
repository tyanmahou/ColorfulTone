#include "KeyConfigScene.h"
#include "Useful.hpp"
#include "SharedDraw.hpp"


//-----------------------------------------------------------------------------------------

ConfigScene::ConfigScene()
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

	if (PlayKey::BigBack().clicked)
	{
		SoundManager::SE::Play(L"desisionLarge");
		changeScene(SceneName::Title, 1000);
	}
}

//--------------------------------------------------------------------------------
//関数：draw
//--------------------------------------------------------------------------------

void ConfigScene::draw()const
{
	TextureAsset(L"canvasBg").draw();
	static SharedDraw::DrawBGLight lights;
	lights.update();
	lights.draw();

	m_config.draw();

	static const String title = L"CONFIG";
	SharedDraw::Sticky(&title, nullptr);

	SceneInfo::Draw(L"Enter:決定 BackSpace:戻る Esc:タイトルに戻る");
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
void ConfigScene::drawFadeOut(double t) const
{
	this->draw();
}
