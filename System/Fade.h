#pragma once
#include "TypeTraits.hpp"
#include<functional>
#include<Siv3D.hpp>
//フェードの種類
namespace Fade
{
	void Default(double t);

	void SmoothCircle(double t);

	// ページめくり
	void FlipPage(double t, std::function<void()> drawble);


	// ページめくり
	void DrawCanvas(double t, std::function<void()> drawble);
	void DrawCanvas(double t, const Color& color = Palette::White);
}

template<class Func, class... Args>
auto FadeIn(Func func, double t, Args&&...args)->std::enable_if_t <Mahou::is_invocable_v<Func, double, Args...>>
{
	func(1.0 - t, std::forward<Args>(args)...);
}
template<class Func, class... Args>
auto FadeOut(Func func, double t, Args&&...args)->std::enable_if_t <Mahou::is_invocable_v<Func, double, Args...>>
{
	func(t, std::forward<Args>(args)...);
}
