#include <core/Play/ColorFx/ColorFx.hpp>
#include <core/Types.hpp>
#include <Siv3D.hpp>

namespace
{
	struct RYB
	{
		double r;
		double y;
		double b;

		RYB operator +(const RYB& v) const noexcept
		{
			return{ (r + v.r), (y + v.y), (b + v.b) };
		}
		RYB operator *(double v) const noexcept
		{
			return{ r * v, y * v, b * v};
		}
		RYB& operator +=(const RYB& v) noexcept
		{
			(*this) = (*this) + v;
			return *this;
		}
	};

	double Min3(double x, double y, double z)
	{
		return Min(x, Min(y, z));
	}
	double Max3(double x, double y, double z)
	{
		return Max(x, Max(y, z));
	}
	RYB ToRYB(const ColorF& color)
	{
		auto [R, G, B, A] = color;
		const double Iw = Min3(R, G, B);
		const double rRGB = R - Iw;
		const double gRGB = G - Iw;
		const double bRGB = B - Iw;

		const double yRGB = Min(rRGB, gRGB);
		const double rRYB = rRGB - yRGB;
		const double yRYB = (yRGB + gRGB) / 2.0;
		const double bRYB = (bRGB + gRGB - yRGB) / 2.0;

		const double maxRGB = Max3(rRGB, gRGB, bRGB);
		const double maxRYB = Max3(rRYB, yRYB, bRYB);

		const double n = maxRYB == 0 ? 1.0 : maxRGB / maxRYB;

		const double rRYB_ = rRYB * n;
		const double yRYB_ = yRYB * n;
		const double bRYB_ = bRYB * n;

		const double Ib = Min3(1 - R, 1 - G, 1 - B);
		return {
			.r = rRYB_ + Ib,
			.y = yRYB_ + Ib,
			.b = bRYB_ + Ib,
		};
	}
	ColorF ToRGB(const RYB& ryb)
	{
		auto [R, Y, B] = ryb;
		const double Ib = Min3(R, Y, B);
		const double rRYB = R - Ib;
		const double yRYB = Y - Ib;
		const double bRYB = B - Ib;

		const double gRYB = Min(yRYB, bRYB);
		const double rRGB = rRYB + yRYB - gRYB;
		const double gRGB = yRYB + gRYB;
		const double bRGB = 2.0 * (bRYB - gRYB);

		const double maxRGB = Max3(rRGB, gRGB, bRGB);
		const double maxRYB = Max3(rRYB, yRYB, bRYB);

		const double n = maxRGB == 0 ? 1.0 : maxRYB / maxRGB;

		const double rRGB_ = rRGB * n;
		const double gRGB_ = gRGB * n;
		const double bRGB_ = bRGB * n;

		const double Iw = Min3(1 - R, 1 - Y, 1 - B);
		return {
			rRGB_ + Iw,
			gRGB_ + Iw,
			bRGB_ + Iw,
		};
	}


	s3d::ColorF GetNoteColor(const ct::NoteType type)
	{
		switch (type) {
		case 1:
		case 11:return s3d::Palette::Red;
		case 2:
		case 12:return s3d::Palette::Blue;
		case 3:
		case 13:return s3d::Palette::Yellow;
		case 4:
		case 14:return s3d::Palette::Lime;
		case 5:
		case 15:return ColorF(1.0, 0.5, 0.0, 1.0);
		case 6:
		case 16:return ColorF(0.5, 0, 1.0, 1.0);
		case 9:
			return s3d::Palette::White;
		case 10:
		case 18:
		{
			HSV hsv{
				Math::Fmod(2.0 * Scene::Time() * 360, 360.0),
				1.0,
				1.0,
				1.0
			};
			return hsv.toColorF();
		}
		}
		return Palette::Black;
	}

	// 一滴
	class ColorDrop
	{
	public:
		ColorDrop(ct::NoteType type) :
			m_type(type),
			m_timer(0.5s, s3d::StartImmediately::Yes)
		{}

		s3d::ColorF getColor() const
		{
			auto from = GetNoteColor(m_type);
			auto to = from;
			to.setA(0);
			double r = m_timer.progress0_1();
			return from.lerp(to, r * r);
		}
		bool isEnd() const
		{
			return m_timer.reachedZero();
		}
	private:
		ct::NoteType m_type;
		s3d::Timer m_timer;
	};
}
namespace ct
{
	class ColorFx::Impl
	{
	public:
		void request(NoteType type)
		{
			m_drops.emplace_back(type);
		}
		s3d::ColorF getColor()
		{
			m_drops.remove_if([](const ColorDrop& d) {
				return d.isEnd();
				});
			s3d::ColorF result(0, 0);
			double alpha = 0;
			RYB rybSum(0, 0, 0);
			for (auto c : m_drops) {
				auto color = c.getColor();
				rybSum += ToRYB(color) * color.a;
				if (alpha < color.a) {
					alpha = color.a;
				}
			}
			rybSum.r = Saturate(rybSum.r);
			rybSum.y = Saturate(rybSum.y);
			rybSum.b = Saturate(rybSum.b);

			result = ToRGB(rybSum);
			result.a = alpha;
			return result;
		}
	private:
		s3d::Array<ColorDrop> m_drops;
	};
	void ColorFx::Request(NoteType type)
	{
		return Instance()->m_pImpl->request(type);
	}
	s3d::ColorF ColorFx::GetColor()
	{
		return Instance()->m_pImpl->getColor();
	}
	ColorFx::ColorFx():
		m_pImpl(std::make_unique<Impl>())
    {
    }
	ColorFx::~ColorFx()
	{
	}
}
