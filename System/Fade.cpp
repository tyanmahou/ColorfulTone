#include"Fade.h"
# include <Siv3D.hpp>

void Fade::Default(double t)
{
	if (t > 0.75)
	{
		Window::BaseClientRect().draw(ColorF(0.0));
		return;
	}

	t *= (1 / 0.75);


	Window::BaseClientRect().draw(ColorF(0.0,t));


}
void Fade::SmoothCircle(double t)
{
	if (t > 0.75)
	{
		Window::BaseClientRect().draw(ColorF(0.0));
		return;
	}

	t *= (1 / 0.75);
	static auto func = [=](float t)
	{
		return ((t - 0.3f)*(t - 0.3f)*(t - 0.3f) + 0.027) / 0.37f;
	};
	Graphics2D::SetStencilState(StencilState::Replace);
	Graphics2D::SetStencilValue(1);
	Circle(Window::BaseCenter(), Window::BaseWidth() * func(1.0 - t)).draw();
	Graphics2D::SetStencilState(StencilState::Test(StencilFunc::NotEqual));

	Window::BaseClientRect().draw(ColorF(0.0, 1));
	Graphics2D::SetStencilState(StencilState::Default);

}
void FadeIn(double t, std::function< void(double) > func){
	func(1.0 - t);
}
void FadeOut(double t, std::function< void(double) > func){
	func(t);
}