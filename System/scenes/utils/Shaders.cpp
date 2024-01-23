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
    const DrawCanvasShader& Shaders::DrawCanvas()
    {
        return Instance()->m_drawCanvasShader;
    }
    Shaders::Shaders():
        m_maskShader(Scene::Size()),
        m_flipPageShader(Scene::Size()),
        m_drawCanvasShader()
    {
    }
}
