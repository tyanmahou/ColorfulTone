#include <core/Play/Effects/JudgeEffect.hpp>
#include <commons/FontName.hpp>
#include <Siv3D.hpp>

namespace ct
{
	JudgeEffect::JudgeEffect(const s3d::String& name, const s3d::Vec2& from):
		m_from(from),
		m_name(name) 
	{}
	bool JudgeEffect::update(double t)
	{
		if (t > 0.5) {
			return false;
		}

		t *= (1 / 0.5);

		const double alpha = 1.0 - t;

		const double offset = -20 * t;

		FontAsset(FontName::Judge)(m_name).drawAt(m_from + Vec2(0, offset), ColorF(0, alpha));

		return true;
	}

}
