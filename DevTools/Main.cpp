#include <Siv3D.hpp>
#include "DevTools.hpp"

void Main()
{
	using namespace ct::dev;
	while (System::Update())
	{
		Vec2 pos{ 0,0 };
		if (SimpleGUI::Button(U"UTF-8エンコード修正", pos)) {
			DevTools::FixFileEncode();
		}
		pos.y += 50;
		if (SimpleGUI::Button(U"PSコンバート", pos)) {
			DevTools::ConvertPS();
		}
	}
}
