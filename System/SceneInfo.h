#pragma once
#include"Game.h"

class SceneInfo 
{
public:
	static void Draw(const String& str )
	{
			const int h = 40;
			Rect(0, 600 - h, 800, h).draw(Palette::Black);
			const auto width = FontAsset(L"info")(str).region().w;
			FontAsset(L"info")(str).draw(780 - width, 600 - h + 10);
	}
private:
	SceneInfo();
};
