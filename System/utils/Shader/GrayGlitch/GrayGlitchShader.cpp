#include <utils/Shader/GrayGlitch/GrayGlitchShader.hpp>
#include <Siv3D.hpp>

namespace
{
    struct ShaderParam
    {
        float timer;
    };
}
namespace ct
{
    class GrayGlitchShader::Impl
    {
    public:
        Impl() :
            m_ps(HLSL(Resource(U"Shaders/grayglitch.ps")))
        {
        }
        void setTimer(double timer)
        {
            m_cb->timer = static_cast<float>(timer);
        }
        [[nodiscard]] s3d::ScopedCustomShader2D start() const
        {
            Graphics2D::SetConstantBuffer(ShaderStage::Pixel, 1, m_cb);
            return s3d::ScopedCustomShader2D{ m_ps };
        }
    private:
        PixelShader m_ps;
        ConstantBuffer<ShaderParam> m_cb;
    };
    GrayGlitchShader::GrayGlitchShader():
        m_pImpl(std::make_unique<Impl>())
    {
    }
    GrayGlitchShader::~GrayGlitchShader()
    {
    }
    s3d::ScopedCustomShader2D GrayGlitchShader::start() const
    {
        double t = s3d::Periodic::Sawtooth0_1(0.5s);
        if (t <= 0.2) {
            t = s3d::Periodic::Triangle0_1(0.2s, t);
        } else {
            t = 0;
        }
        m_pImpl->setTimer(t);
        return m_pImpl->start();
    }
}
