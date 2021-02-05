#include"VideoAsset.h"
#include"StartAnime.h"
#include "Useful.hpp"

void StartAnime::Draw(double t)
{
	// 0.7 0.2 0.1
	if (t <= 0) {
		return;
	}
	const double alpha = Min(1.0, -10 * t + 10);
	const ColorF color(1, alpha);

	for (int i : step_until(1, 7)) {
		double tmp = Clamp(10 * (t - i/10.0 + 0.1), 0.0, 1.0);
		bool isOdd = i % 2;
		double v = isOdd ? 0 : 1 - tmp;
		TextureAsset(L"start0" + Format(i)).uv(0, v, 1, tmp).draw({0, 600*v},color);
	}
	//double tmp = Min(1.0, 10 * t);
	//TextureAsset(L"start01").uv(0, 0, 1, tmp).draw(color);

	//tmp = Clamp(10 * (t - 0.1), 0.0, 1.0);
	//TextureAsset(L"start02").uv(0, 1-tmp, 1, tmp).draw({ 0, 600 - tmp * 600 }, color);
}
