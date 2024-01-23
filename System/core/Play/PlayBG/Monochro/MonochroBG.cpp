#include <core/Play/PlayBG/Monochro/MonochroBG.hpp>

namespace ct
{
    MonochroBG::MonochroBG(const s3d::Color& color) :
        m_color(color)
    {
    }
    void MonochroBG::init([[maybe_unused]] const NotesData& notes)
    {
    }

    void MonochroBG::apply([[maybe_unused]] double count) const
    {
        Scene::Rect().draw(m_color);
    }
}