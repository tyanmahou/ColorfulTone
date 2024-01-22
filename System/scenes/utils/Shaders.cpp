#include <scenes/utils/Shaders.hpp>
#include <Siv3D.hpp>

namespace ct
{
    const MaskShader& Shaders::Mask()
    {
        return Instance()->m_maskShader;
    }
    const FlipPageShader& Shaders::FlipPage()
    {
        return Instance()->m_flipPageShader;
    }
    Shaders::Shaders():
        m_maskShader(Scene::Size()),
        m_flipPageShader(Scene::Size())
    {
    }
}
