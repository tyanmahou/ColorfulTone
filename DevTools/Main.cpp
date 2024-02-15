#include <Siv3D.hpp>
#include "DevTools.hpp"
#include <utils/Coro/Fiber/FiberHolder.hpp>
#include <scenes/utils/SharedDraw/LoadingCircle.hpp>

void Main()
{
	using namespace ct;
	using namespace ct::dev;

	Coro::FiberHolder<void> fiber;
	while (System::Update())
	{
		bool loading = fiber.resume();
		if (loading) {
			MouseL.clearInput();
		}
		Vec2 pos{ 0,0 };
		if (SimpleGUI::Button(U"UTF-8エンコード修正", pos)) {
			DevTools::FixFileEncode();
		}
		pos.y += 50;
		if (SimpleGUI::Button(U"PSコンバート", pos)) {
			DevTools::ConvertPS();
		}
		pos.y += 50;
		if (SimpleGUI::Button(U"譜面解析", pos)) {
			fiber.reset(DevTools::AnalyzeAsync);
		}
		if (loading) {
			Scene::Rect().draw(ColorF(0, 0.5));
			SharedDraw::LoadingCircle::DrawMain(ColorF(1, 0.9));
		}
	}
}
