﻿#include <Siv3D.hpp>
#include "DevTools.hpp"
#include <utils/Coro/Fiber/FiberHolder.hpp>
#include <scenes/utils/SharedDraw/LoadingCircle.hpp>
#include <utils/Notify/Notify.hpp>

using namespace ct;
using namespace ct::dev;

class DevMain
{
public:
	void update()
	{
		bool loading = m_fiber.resume();
		if (loading) {
			MouseL.clearInput();
		}
		Vec2 pos{ 0,0 };
		if (SimpleGUI::Button(U"UTF-8エンコード修正", pos)) {
			if (DevTools::FixFileEncode()) {
				m_notify.show(U"COMPLETED");
			} else {
				m_notify.error(U"FAILED");
			}
		}
		pos.y += 50;
		if (SimpleGUI::Button(U"PSコンバート", pos)) {
			if (DevTools::ConvertPS()) {
				m_notify.show(U"COMPLETED");
			} else {
				m_notify.error(U"FAILED");
			}
		}
		pos.y += 50;
		SimpleGUI::CheckBox(m_isOfficialFilter, U"公式フィルタ", pos + Vec2{ 200, 0 });
		if (SimpleGUI::Button(U"譜面解析", pos)) {
			m_fiber.reset(std::bind_back(&DevMain::onAnalyze, this));
		}
		if (loading) {
			Scene::Rect().draw(ColorF(0, 0.5));
			SharedDraw::LoadingCircle::DrawMain(ColorF(1, 0.9));
		}
		m_notify.update(Scene::DeltaTime());
		m_notify.draw();
	}

	Coro::Fiber<void> onAnalyze()
	{
		bool result = co_await DevTools::AnalyzeAsync(m_isOfficialFilter);
		if (result) {
			m_notify.show(U"COMPLETED");
		} else {
			m_notify.error(U"FAILED");
		}
	}
private:
	bool m_isOfficialFilter = true;
	Coro::FiberHolder<void> m_fiber;
	Notify m_notify;
};
void Main()
{
	DevMain devMain;

	while (System::Update())
	{
		devMain.update();
	}
}
