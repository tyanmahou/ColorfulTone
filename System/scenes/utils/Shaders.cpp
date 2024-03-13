#include <scenes/utils/Shaders.hpp>
#include <Siv3D.hpp>

namespace ct
{
    const MaskShader& Shaders::Mask(size_t useBufferIndex)
    {
        return Instance()->m_maskShader.useBuffer(useBufferIndex);
    }
    const FlipPageShader& Shaders::FlipPage()
    {
        return Instance()->m_flipPageShader;
    }
    const DrawCanvasShader& Shaders::DrawCanvas()
    {
        return Instance()->m_drawCanvasShader;
    }
    const MainBgShader& Shaders::MainBg()
    {
        return Instance()->m_mainBgShader;
    }
    const BlendShader& Shaders::Blend()
    {
        return Instance()->m_blendShader;
    }
    const Gizi3DCamera& Shaders::Gizi3D()
    {
        return Instance()->m_gizi3D;
    }
    Shaders::Shaders():
        m_maskShader(Scene::Size(), 4),
        m_flipPageShader(Scene::Size()),
        m_drawCanvasShader(),
        m_mainBgShader(),
        m_blendShader(Scene::Size()),
        m_gizi3D(Scene::Size())
    {
    }
}
