#pragma once
#include <utils/Singleton/DynamicSingleton.hpp>
#include <utils/Shader/Mask/MaskShader.hpp>
#include <utils/Shader/FlipPage/FlipPageShader.hpp>

namespace ct
{
    class Shaders : protected DynamicSingleton<Shaders>
    {
        friend class DynamicSingleton<Shaders>;
    public:
        static const MaskShader& Mask();
        static const FlipPageShader& FlipPage();
    private:
        Shaders();
        MaskShader m_maskShader;
        FlipPageShader m_flipPageShader;
    };
}