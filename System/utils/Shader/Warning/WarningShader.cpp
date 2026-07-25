#include <utils/Shader/Warning/WarningShader.hpp>
#include <Siv3D.hpp>

namespace
{
    struct ShaderParam
    {
        Float3 color;
        float blend;
        float noiseRate;
        float timer;
    };
}
namespace ct
{
    class WarningShader::Impl
    {
    public:
        Impl(const s3d::Size& size):
            m_ps(HLSL(Resource(U"Shaders/warning.ps"))),
            m_rt(size)
        {
        }
        void setColor(const s3d::ColorF& color)
        {
            m_cb->color = color.toFloat4().xyz();
        }
        void setBlend(double blend)
        {
            m_cb->blend = static_cast<float>(blend);
        }
        void setNoiseRate(double noiseRate)
        {
            m_cb->noiseRate = static_cast<float>(noiseRate);
        }
        void setTime(double time)
        {
            m_cb->timer = static_cast<float>(time);
        }
        void apply(std::function<void()> drawable) const
        {
            {
                ScopedRenderTarget2D srt(m_rt);
                m_rt.clear(ColorF(1, 1));
                drawable();
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
    WarningShader::WarningShader(const s3d::Size& size):
        m_pImpl(std::make_unique<Impl>(size))
    {
    }
    WarningShader::~WarningShader()
    {
    }
    const WarningShader& WarningShader::setColor(const s3d::ColorF& color) const
    {
        m_pImpl->setColor(color);
        return *this;
    }
    const WarningShader& WarningShader::setNoiseRate(double noiseRate) const
    {
        m_pImpl->setNoiseRate(noiseRate);
        return *this;
    }

    const WarningShader& WarningShader::setBlend(double blend) const
    {
        m_pImpl->setBlend(blend);
        return *this;
    }
    const WarningShader& WarningShader::setTime(double time) const
    {
        m_pImpl->setTime(time);
        return *this;
    }
    void WarningShader::apply(std::function<void()> drawable) const
    {
        m_pImpl->apply(std::move(drawable));
    }
}
