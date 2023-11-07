#include <scenes/utils/Shaders.hpp>
#include <Siv3D.hpp>

namespace ct
{
    const MaskShader& Shaders::Mask()
    {
        return Instance()->m_maskShader;
    }
    Shaders::Shaders():
        m_maskShader(Scene::Size())
    {
    }
}
