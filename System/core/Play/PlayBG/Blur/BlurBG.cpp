#include <core/Play/PlayBG/Blur/BlurBG.hpp>
#include <core/Data/MusicData/MusicData.hpp>
#include <Siv3D.hpp>

namespace ct
{
    BlurBG::BlurBG()
    {
    }
    void BlurBG::init(const NotesData& notes)
    {
        TextureRegion region = notes.getMusic().getTexture().resized(800, 800);

        RenderTexture irt{ 800, 800 };
        RenderTexture rt{ 800, 800 };
        RenderTexture irt2{ 200, 200 };
        RenderTexture rt2{ 200, 200 };
        Shader::GaussianBlur(region, irt, rt);
        Shader::Downsample(rt, rt2);
        Shader::GaussianBlur(rt2, irt2, rt2);
        m_texturRegion = rt2.resized(800, 800);
    }

    void BlurBG::apply([[maybe_unused]] double count) const
    {
        m_texturRegion.drawAt(Scene::CenterF());
    }
}
