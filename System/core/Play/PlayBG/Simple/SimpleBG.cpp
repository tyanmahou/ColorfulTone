#include <core/Play/PlayBG/Simple/SimpleBG.hpp>

namespace ct
{
    SimpleBG::SimpleBG(const s3d::Color& color) :
        m_color(color)
    {
        m_color.setA(240);
    }
    void SimpleBG::init(const NotesData& notes)
    {
        m_texture = notes.getMusic()->getTexture().resized(800, 800);
    }

    void SimpleBG::apply([[maybe_unused]] double count) const
    {
        m_texture.draw(Scene::CenterF());
        Scene::Rect().draw(m_color);
    }
}