#pragma once
#include <functional>

namespace Fade
{
	void Default(double t);

	//�t�F�[�h�̎��
	void SmoothCircle(double t);
}
void FadeIn(double t, std::function< void(double) > func);
void FadeOut(double t, std::function< void(double) > func);
