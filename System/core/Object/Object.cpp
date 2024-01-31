#include <core/Object/Object.hpp>
#include <core/Object/Stop/Stop.hpp>
#include <commons/Game/GameConfig.hpp>
#include <Siv3D.hpp>
namespace ct
{
    void Object::draw(double nowCount, double scrollRate) const
    {
        const double count = m_drawCount - nowCount;

        if (m_isActive) {
            diffDraw(count, scrollRate);
        }
    }
    void Object::addStopCount(const StopRange& stopRange)
    {
        if (m_count > stopRange.m_judgeCount) {

            const auto range = std::min(m_count - stopRange.m_judgeCount, stopRange.m_rangeCount);

            m_drawCount -= range;
        }
    }
    bool Object::CanDraw(const Vec2& pos)
    {
        const double scale = Game::Config().m_playScale;
        if (pos.x < 350.0 - 400.0 / scale || pos.x>450.0 + 400 / scale || pos.y < 250 - 300.0 / scale || pos.y>350.0 + 300.0 / scale) {
            return false;
        }
        return true;
    }
}
