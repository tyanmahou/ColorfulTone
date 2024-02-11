#pragma once
#include <utils/Singleton/DynamicSingleton.hpp>
#include <utils/Shader/Mask/MaskShader.hpp>
#include <utils/Shader/FlipPage/FlipPageShader.hpp>
#include <utils/Shader/DrawCanvas/DrawCanvasShader.hpp>
#include <utils/Shader/MainBg/MainBgShader.hpp>
#include <utils/Shader/Blend/BlendShader.hpp>

namespace ct
{
    class Shaders : protected DynamicSingleton<Shaders>
    {
        friend class DynamicSingleton<Shaders>;
    public:
        static const MaskShader& Mask(size_t useBufferIndex = 0);
        static const FlipPageShader& FlipPage();
        static const DrawCanvasShader& DrawCanvas();
        static const MainBgShader& MainBg();
        static const BlendShader& Blend();
    private:
        Shaders();
        MaskShader m_maskShader;
        FlipPageShader m_flipPageShader;
        DrawCanvasShader m_drawCanvasShader;
        MainBgShader m_mainBgShader;
        BlendShader m_blendShader;
    };
}