#pragma once
#include"Util.h"
namespace SharedDraw
{
	template<class T>
	class Select
	{
	private:
		double m_offset = 0;
		uint32 m_width = 280;
		std::function<Color(const T&)> m_colorCallback;
		std::function<void(const T&, Vec2 pos)> m_drawable;
	public:
		Select() :
			m_colorCallback([](const T&) {return Palette::White; }),
			m_drawable([](const T&, Vec2) {})
		{}
		Select& setColorCallBack(std::function<Color(const T&)> callback)
		{
			this->m_colorCallback = callback;
			return *this;
		}
		Select& setDrawble(std::function<void(const T&, Vec2 pos)> callback)
		{
			this->m_drawable = callback;
			return *this;
		}
		Select& setWidth(uint32 width)
		{
			this->m_width = width;
			return *this;
		}
		Select& setOffset(double offset)
		{
			this->m_offset = offset;
			return *this;
		}
		void draw(
			const Array<T>& ar,
			uint32 select,
			std::function<const String&(const T&)> strCallBack
		)const {
			const uint32 size = ar.size();
			if (size == 0)
			{
				return;
			}
			for (int i = 0; i < 10; ++i)
			{
				double offset = i == 4 ? m_offset : 0.0;
				Color uiColor = i == 4 ? Palette::Yellow : Palette::White;
				const Vec2 pos{ 430 + offset, 60 * i };
				TextureAsset(L"levelbg").draw(pos, uiColor);
				const auto index = (select + i - 4 + size * Abs(i - 4)) % size;

				const auto color = m_colorCallback(ar[index]);
				util::ContractionDrawbleString(
					FontAsset(L"selectMusics")(strCallBack(ar[index])),
					pos + Vec2{ 80,13 },
					m_width,
					color,
					false
				);
				m_drawable(ar[index], pos);
			}
			TextureAsset(L"levelMask").draw(430, 0, Palette::Black);
		}
	};

	class DrawBGLight
	{
	private:
		Effect m_effect;
	public:
		void update();
		void draw()const;
	};

	void Sticky(const String* blueText, const String* redText = nullptr);

	class JacketInfo
	{
		const FontAsset m_font12;
		const FontAsset m_font16b;
	public:
		JacketInfo();
		const JacketInfo& drawLine() const;
		//タイトル
		const JacketInfo& drawTitle(const String& title, const Color& color = Palette::Black) const;
		// サブタイトル
		const JacketInfo& drawSub(const String& sub, const Color& color = Palette::Black) const;
		// 詳細
		const JacketInfo& drawDetail(const String& detail, const Color& color = Palette::Black) const;
	};
}

namespace Constants
{
	constexpr double JacketWidth = 350;  // 曲情報の幅
	constexpr double JacketCenter = 220; // 曲情報の中心
}

