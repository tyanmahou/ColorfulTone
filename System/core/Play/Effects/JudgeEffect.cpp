#include <core/Play/Effects/JudgeEffect.hpp>
#include <core/Data/Score/Score.hpp>
#include <commons/FontName.hpp>
#include <Siv3D.hpp>

namespace ct
{
	JudgeEffect::JudgeEffect(Score::Judge judge, s3d::int64 diff, const s3d::Vec2& from)
		: JudgeEffect(JudgeStr(judge), from)
	{
		if (judge == Score::Judge::Good || judge == Score::Judge::Great) {
			if (diff > 0) {
				m_color.r = 160 / 255.0;
			} else if (diff < 0) {
				m_color.b = 160 / 255.0;
			}
		}
	}
	JudgeEffect::JudgeEffect(s3d::StringView name, const s3d::Vec2& from, const s3d::ColorF& color):
		m_from(from),
		m_name(name),
		m_color(color)
	{}
	bool JudgeEffect::update(double t)
	{
		if (t > 0.5) {
			return false;
		}

		t *= (1 / 0.5);

		double alpha = 1.0 - t;
		alpha = 1 - (1 - alpha) * (1 - alpha);
		const double offset = -20 * t;

		ColorF c = m_color;
		c.setA(alpha);

		ColorF back(Palette::White, alpha * 0.5);

		FontAsset(FontName::Judge)(m_name).drawAt(m_from + Vec2(0, offset) + Vec2{ 1,1 }, back);
		FontAsset(FontName::Judge)(m_name).drawAt(m_from + Vec2(0, offset), c);
		return true;
	}

}
