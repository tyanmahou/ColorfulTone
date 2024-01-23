#include <core/Play/PlayBG/Default/DefaultBG.hpp>
#include <scenes/utils/Shaders.hpp>

namespace ct
{
    void DefaultBG::init(const NotesData& notes)
    {
        m_texture = notes.getMusic()->getTexture().resized(1920, 1920);
    }

    void DefaultBG::apply(double count) const
    {
        auto shader = Shaders::MainBg()
            .setTimer(count)
            .start();
        m_texture.drawAt(Scene::CenterF());
    }
}
