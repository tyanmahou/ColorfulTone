#include <utils/Shader/Blend/BlendShader.hpp>
#include <Siv3D.hpp>

namespace
{
    struct ShaderParam
    {
        Float4 color;
    };
}
namespace ct
{
    class BlendShader::Impl
    {
    public:
        Impl(const s3d::Size& size):
            m_ps(HLSL(U"Shaders/blend.ps")),
            m_rt(size)
        {
        }
        void setColor(const s3d::ColorF& color)
        {
            m_cb->color = color.toFloat4();
        }
        void apply(std::function<void()> drawble) const
        {
            {
                ScopedRenderTarget2D srt(m_rt);
                m_rt.clear(ColorF(1, 1));
                drawble();
            }
            {
                Graphics2D::SetConstantBuffer(ShaderStage::Pixel, 1, m_cb);
                ScopedCustomShader2D scopedPs(m_ps);
                m_rt.draw();
            }
        }
    private:
        PixelShader m_ps;
        RenderTexture m_rt;
        ConstantBuffer<ShaderParam> m_cb;
    };
    BlendShader::BlendShader(const s3d::Size& size):
        m_pImpl(std::make_unique<Impl>(size))
    {
    }
    ct::BlendShader::~BlendShader()
    {
    }
    const BlendShader& BlendShader::setColor(const s3d::ColorF& color) const
    {
        m_pImpl->setColor(color);
        return *this;
    }
    void BlendShader::apply(std::function<void()> drawble) const
    {
        m_pImpl->apply(std::move(drawble));
    }
}
