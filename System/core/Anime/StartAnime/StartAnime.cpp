#include <core/Anime/StartAnime/StartAnime.hpp>
#include <Siv3D.hpp>

namespace ct
{
	void StartAnime::Draw(double t)
	{
		// 0.7 0.2 0.1
		if (t <= 0) {
			return;
		}
		const double alpha = Min(1.0, -10 * t + 10);
		const ColorF color(1, alpha);

		for (int32 i : step_until(1, 7)) {
			double tmp = Clamp(10 * (t - i / 10.0 + 0.1), 0.0, 1.0);
			bool isOdd = i % 2;
			double v = isOdd ? 0 : 1 - tmp;
			TextureAsset(U"start0" + Format(i)).uv(0, v, 1, tmp).draw({ 0, 600 * v }, color);
		}
	}
}
