#include <scenes/Scene/Preview/GUI/Pulldown.hpp>
#include <Siv3D.hpp>

namespace ct::GUI
{
    Pulldown::Pulldown(size_t& index, bool& isOpen):
        m_index(index),
        m_isOpen(isOpen)
    {
    }
    s3d::RectF Pulldown::draw(const s3d::Array<s3d::String>& items, const s3d::Vec2& pos) const
    {
		static constexpr Size Padding{ 8, 2 };
		static constexpr int32 DownButtonSize = 16;

		RectF region{ pos , (m_maxItemWidth + (Padding.x * 3 + DownButtonSize)),(m_font.height() + Padding.y * 2)};

		if (m_enabled) {
			// 更新
			auto update = [&]() {
				if (items.isEmpty()) {
					return false;
				}
				if (region.mouseOver()) {
					Cursor::RequestStyle(CursorStyle::Hand);
				}
				if (region.leftClicked()) {
					m_isOpen = (not m_isOpen);
					MouseL.clearInput();
				}

				if (not m_isOpen) {
					return false;
				}

				Vec2 itemPos = region.pos.movedBy(0, region.h);

				for (size_t i = 0; i < items.size(); ++i) {
					const RectF itemRect{ itemPos, region.w, region.h };

					if (itemRect.mouseOver()) {
						Cursor::RequestStyle(CursorStyle::Hand);
					}
					if (itemRect.leftClicked()) {
						size_t prevIndex = m_index;
						m_index = i;
						m_isOpen = false;
						MouseL.clearInput();
						return prevIndex != m_index;
					}

					itemPos.y += region.h;
				}
				if (MouseL.down()) {
					m_isOpen = (not m_isOpen);
				}
				return false;
				};
			if (update()) {
				if (m_onChangeCallback) {
					m_onChangeCallback(m_index);
				}
			}
		}
		region.draw(m_listMouseOverColor);

		if (items.isEmpty()) {
			return region;
		}
		const ColorF& textColor = m_enabled ? m_textColor : m_disbaledTextColor;

		region.drawFrame(1, 0, m_isOpen ? Palette::Orange : Palette::Gray);

		m_font(items[m_index]).draw((region.pos + Padding), textColor);

		Triangle{ (region.rightX() - DownButtonSize / 2.0 - Padding.x), (region.y + region.h / 2.0),
			(DownButtonSize * 0.5), 180_deg }.draw(textColor);

		if (not m_isOpen) {
			return region;
		}

		Vec2 itemPos = region.bl();

		const RectF backRect{ itemPos, region.w, (region.h * items.size()) };

		backRect.drawShadow({ 1, 1 }, 5, 0).draw(m_listBackColor);

		for (const auto& item : items) {
			const RectF rect{ itemPos, region.size };

			if (rect.mouseOver()) {
				rect.draw(m_listMouseOverColor);
			}

			m_font(item).draw((itemPos + Padding), textColor);

			itemPos.y += region.h;
		}

		backRect.drawFrame(1, 0, Palette::Gray);
		return region;
    }
}

