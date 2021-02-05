#include"ColorfulTone.h"
#include"Game.h"
#include"TitleApi.hpp"

bool ColorfulTone::update()
{
	return Game::UpdateAndDraw();
}

ColorfulTone::ColorfulTone() :
	AppBase(L"ColorfulTone")
{

	Profiler::EnableWarning(false);

	//バージョンチェック
	TitleApi::CheckVersion();
}

ColorfulTone::~ColorfulTone()
{}
