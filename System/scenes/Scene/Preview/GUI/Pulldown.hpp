#include <Siv3D/Font.hpp>
#include <Siv3D/Array.hpp>

namespace ct::GUI
{
	class Pulldown
	{
	public:
		Pulldown(size_t& index, bool& isOpen);

		Pulldown& setEnabled(bool enabled)
		{
			m_enabled = enabled;
			return *this;
		}
		Pulldown& setFont(const s3d::Font& font)
		{
			m_font = font;
			return *this;
		}
		Pulldown& setMaxItemWidth(double w)
		{
			m_maxItemWidth = w;
			return *this;
		}
		Pulldown& setTextColor(const s3d::ColorF& color)
		{
			return this->setTextColor(color, color);
		}
		Pulldown& setTextColor(const s3d::ColorF& enabled, const s3d::ColorF& disabled)
		{
			m_textColor = enabled;
			m_disbaledTextColor = disabled;
			return *this;
		}
		Pulldown& setListMouseOverColor(const s3d::ColorF& color)
		{
			m_listMouseOverColor = color;
			return *this;
		}
		Pulldown& setListBackColor(const s3d::ColorF& color)
		{
			m_listBackColor = color;
			return *this;
		}
		Pulldown& setOnChange(std::function<void(size_t)>&& callback)
		{
			m_onChangeCallback = std::move(callback);
			return *this;
		}
		s3d::RectF draw(const s3d::Array<s3d::String>& items, const s3d::Vec2& pos) const;
	private:
		size_t& m_index;
		bool& m_isOpen;

		bool m_enabled = true;
		s3d::Font m_font;
		double m_maxItemWidth = 0;

		s3d::ColorF m_textColor;
		s3d::ColorF m_disbaledTextColor;
		s3d::ColorF m_listMouseOverColor;
		s3d::ColorF m_listBackColor;

		std::function<void(size_t)> m_onChangeCallback;
	};
}