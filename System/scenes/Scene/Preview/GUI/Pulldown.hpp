# include <Siv3D.hpp>

namespace ct
{
	class Pulldown
	{
	public:
		Pulldown() = default;

		Pulldown(const Font& font, double maxWidth, const Vec2& pos)
			: m_font{ font }
			, m_maxitemWidth{ maxWidth }
			, m_rect{ getRect(pos) } {}

		bool isEmpty() const noexcept
		{
			return m_items.empty();
		}

		void setPos(const Vec2& pos) noexcept
		{
			m_rect.setPos(pos);
		}

		[[nodiscard]]
		const RectF& getRect() const noexcept
		{
			return m_rect;
		}

		[[nodiscard]]
		size_t getIndex() const noexcept
		{
			return m_index;
		}

		void setItems(const Array<String>& items)
		{
			m_items = items;
			if (m_items.isEmpty()) {
				m_index = 0;
				return;
			}
			m_index %= m_items.size();
		}
		[[nodiscard]]
		const Array<String>& getItems() const noexcept
		{
			return m_items;
		}

		bool update()
		{
			if (isEmpty()) {
				return false;
			}
			if (m_rect.leftClicked()) {
				m_isOpen = (not m_isOpen);
				MouseL.clearInput();
			}

			if (not m_isOpen) {
				return false;
			}

			Vec2 itemPos = m_rect.pos.movedBy(0, m_rect.h);

			for (size_t i = 0; i < m_items.size(); ++i) {
				const RectF itemRect{ itemPos, m_rect.w, m_rect.h };

				if (itemRect.leftClicked()) {
					size_t prevIndex = m_index;
					m_index = i;
					m_isOpen = false;
					MouseL.clearInput();
					return prevIndex != m_index;
				}

				itemPos.y += m_rect.h;
			}
			if (MouseL.down()) {
				m_isOpen = (not m_isOpen);
			}
			return false;
		}

		RectF draw(const ColorF& textColor, const ColorF& backColor, const ColorF& listColor) const
		{
			m_rect.draw(listColor);

			if (isEmpty()) {
				return m_rect;
			}

			m_rect.drawFrame(1, 0, m_isOpen ? Palette::Orange : Palette::Gray);

			m_font(m_items[m_index]).draw((m_rect.pos + Padding), textColor);

			Triangle{ (m_rect.rightX() - DownButtonSize / 2.0 - Padding.x), (m_rect.y + m_rect.h / 2.0),
				(DownButtonSize * 0.5), 180_deg }.draw(textColor);

			if (not m_isOpen) {
				return m_rect;
			}

			Vec2 itemPos = m_rect.bl();

			const RectF backRect{ itemPos, m_rect.w, (m_rect.h * m_items.size()) };

			backRect.drawShadow({ 1, 1 }, 5, 0).draw(backColor);

			for (const auto& item : m_items) {
				const RectF rect{ itemPos, m_rect.size };

				if (rect.mouseOver()) {
					rect.draw(listColor);
				}

				m_font(item).draw((itemPos + Padding), textColor);

				itemPos.y += m_rect.h;
			}

			backRect.drawFrame(1, 0, Palette::Gray);
			return m_rect;
		}

	private:

		static constexpr Size Padding{ 8, 2 };

		static constexpr int32 DownButtonSize = 16;

		Font m_font;

		Array<String> m_items;

		size_t m_index = 0;

		double m_maxitemWidth = 0;

		RectF m_rect{ 0 };

		bool m_isOpen = false;

		[[nodiscard]]
		double getMaxItemWidth() const
		{
			double result = 0.0;

			for (const auto& item : m_items) {
				result = Max(result, (m_font(item).region().w));
			}

			return result;
		}

		[[nodiscard]]
		RectF getRect(const Vec2& pos) const noexcept
		{
			const double fontHeight = m_font.height();

			return{
				pos,
				(m_maxitemWidth + (Padding.x * 3 + DownButtonSize)),
				(fontHeight + Padding.y * 2)
			};
		}
	};

}
