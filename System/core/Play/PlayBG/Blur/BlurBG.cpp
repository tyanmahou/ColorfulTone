#include <core/Play/PlayBG/Blur/BlurBG.hpp>

namespace ct
{
    BlurBG::BlurBG() :
        m_internalTexture(Size{800, 800}),
        m_renderTexture(Size{ 800, 800 })
    {
    }
    void BlurBG::init(const NotesData& notes)
    {
        TextureRegion region = notes.getMusic()->getTexture().resized(800, 800);
        Shader::GaussianBlur(region, m_internalTexture, m_renderTexture);
    }

    void BlurBG::apply([[maybe_unused]] double count) const
    {
        m_renderTexture.drawAt(Scene::CenterF());
    }
}
