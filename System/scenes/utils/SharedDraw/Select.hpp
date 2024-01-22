#pragma once
#include <functional>
#include <commons/FontName.hpp>
#include <scenes/utils/Util.hpp>
#include <Siv3D/TextureAsset.hpp>

namespace ct::SharedDraw
{
	template<class T>
	class Select
	{
	public:
		Select() :
			m_colorCallback([](const T&) {return s3d::Palette::White; }),
			m_drawable([](const T&, s3d::Vec2) {})
		{}
		Select& setColorCallBack(std::function<s3d::Color(const T&)> callback)
		{
			this->m_colorCallback = callback;
			return *this;
		}
		Select& setDrawble(std::function<void(const T&, s3d::Vec2 pos)> callback)
		{
			this->m_drawable = callback;
			return *this;
		}
		Select& setWidth(s3d::uint32 width)
		{
			this->m_width = width;
			return *this;
		}
		Select& setOffset(double offset)
		{
			this->m_offset = offset;
			return *this;
		}
		Select& setLoop(bool isLoop)
		{
			m_isLoop = isLoop;
			return *this;
		}
		void draw(
			const s3d::Array<T>& ar,
			s3d::uint32 select,
			std::function<const s3d::String& (const T&)> strCallBack
		)const {
			const s3d::uint32 size = static_cast<s3d::uint32>(ar.size());
			if (size == 0) {
				return;
			}
			for (s3d::int32 i = 0; i < 10; ++i) {
				double offset = i == 4 ? m_offset : 0.0;
				s3d::Color uiColor = i == 4 ? s3d::Palette::Yellow : s3d::Palette::White;
				const s3d::Vec2 pos{ 430 + offset, 60 * i };
				s3d::TextureAsset(U"levelbg").draw(pos, uiColor);
				const s3d::int32 noneLoopIndex = static_cast<s3d::int32>(select) + i - 4;
				if (!m_isLoop && (noneLoopIndex >= static_cast<s3d::int32>(size) || noneLoopIndex < 0)) {
					// 範囲外はスルー
					continue;
				}

				const auto index = (noneLoopIndex + size * Abs(i - 4)) % size;

				const auto color = m_colorCallback(ar[index]);
				ContractionDrawbleString(
					FontAsset(FontName::SelectMusic)(strCallBack(ar[index])),
					pos + Vec2{ 80,13 },
					m_width,
					color,
					false
				);
				m_drawable(ar[index], pos);
			}
			s3d::TextureAsset(U"levelMask").draw(430, 0, s3d::Palette::Black);
		}
	private:
		double m_offset = 0;
		s3d::uint32 m_width = 280;
		std::function<s3d::Color(const T&)> m_colorCallback;
		std::function<void(const T&, s3d::Vec2 pos)> m_drawable;
		bool m_isLoop = true;
	};
}