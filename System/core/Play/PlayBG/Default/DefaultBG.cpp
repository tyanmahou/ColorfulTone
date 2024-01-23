#include <core/Play/PlayBG/Default/DefaultBG.hpp>
#include <scenes/utils/Shaders.hpp>
#include <Siv3D.hpp>

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

        s3d::ScopedRenderStates2D sampler(SamplerState::RepeatLinear);
        m_texture.drawAt(Scene::CenterF());
    }
}
