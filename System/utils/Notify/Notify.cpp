#include <utils/Notify/Notify.hpp>
#include <Siv3D.hpp>

namespace ct
{
    constexpr double lifeTime = 3.0;

    void Notify::show(s3d::StringView message, s3d::StringView detail)
    {
        const double currentIndex = (m_entries.empty() ? 0.0 : m_entries.back().currentIndex + 1.0);
        const double velocity = (m_entries.empty() ? 0.0 : m_entries.back().velocity);

        m_entries << Entry{
            .message = String{ message },
            .detail = String{ detail },
            .time = 0.0,
            .currentIndex = currentIndex,
            .velocity = velocity
        };
    }
    void Notify::update(double dt)
    {
        for (auto&& e : m_entries) {
            e.time += dt;
        }
        m_entries.remove_if([](const Entry& entry) {
            return entry.time > lifeTime;
            });
        for (auto&& [index, e] : s3d::IndexedRef(m_entries)) {
            e.currentIndex = Math::SmoothDamp(
                e.currentIndex,
                static_cast<double>(index), 
                e.velocity, 
                0.15, 
                s3d::none,
                dt
            );
        }
    }
    void Notify::draw() const
    {
        const Font& font = SimpleGUI::GetFont();
        for (auto&& e : m_entries) {
            double xScale = 1.0;
            double alpha = 1.0;

            if (e.time < 0.2) {
                xScale = alpha = (e.time / 0.2);
            } else if ((lifeTime - 0.4) < e.time) {
                alpha = ((lifeTime - e.time) / 0.4);
            }

            alpha = EaseOutExpo(alpha);
            xScale = EaseOutExpo(xScale);

            constexpr ColorF BackgroundColor{ 0.0, 0.8 };
            constexpr ColorF TextColor{ 1.0 };
            constexpr ColorF SuccessColor{ 0.0, 0.78, 0.33 };
            constexpr double Width = 350;

            ColorF backgroundColor = BackgroundColor;
            backgroundColor.a *= alpha;

            ColorF textColor = TextColor;
            textColor.a *= alpha;

            const RectF rect{ 0, (50 + e.currentIndex * 52), (Width * xScale), 51 };
            rect.draw(backgroundColor);

            ColorF color = SuccessColor;
            color.a *= alpha;
            font(U"\U000F0E1E").draw(26, Arg::leftCenter = rect.leftCenter().movedBy(8, -1), color);
            font(e.message).draw(18, rect.pos + Vec2{40, 2}, color);
            font(e.detail).draw(12, rect.pos + Vec2{ 40, 26 }, textColor);
        }
    }
}
