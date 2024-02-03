#include <utils/Shader/FlipPage/FlipPageShader.hpp>
#include <Siv3D.hpp>

namespace
{
    struct ShaderParam
    {
        float timer;
        int32 in;
    };
}
namespace ct
{
    class FlipPageShader::Impl
    {
    public:
        Impl(const Size& size):
            m_ps(HLSL(Resource(U"Shaders/flipPage.ps"))),
            m_tex(size)
        {}
        void setTimer(double timer) 
        {
            m_cb->timer = static_cast<float>(timer);
        }
        void setIn(bool in)
        {
            m_cb->in = in ? 1 : 0;
        }
        ScopedCustomShader2D start() const
        {
            Graphics2D::SetConstantBuffer(ShaderStage::Pixel, 1, m_cb);
            return ScopedCustomShader2D{ m_ps };
        }
        void apply(std::function<void()> drawble) const
        {
            {
                Transformer2D t2d(Mat3x2::Identity(), s3d::Transformer2D::Target::SetLocal);
                ScopedRenderTarget2D target(m_tex);
                m_tex.clear(ColorF(0, 1));
                drawble();
            }
            {
                auto scoped = this->start();
                m_tex.draw();
            }
        }
    private:
        PixelShader m_ps;
        ConstantBuffer<ShaderParam> m_cb;
        RenderTexture m_tex;
    };
    FlipPageShader::FlipPageShader(const Size& size):
        m_pImpl(std::make_unique<Impl>(size))
    {
    }
    FlipPageShader::~FlipPageShader()
    {
    }
    const FlipPageShader& FlipPageShader::setTimer(double timer) const
    {
        m_pImpl->setTimer(timer);
        return *this;
    }
    const FlipPageShader& FlipPageShader::setIn(bool in) const
    {
        m_pImpl->setIn(in);
        return *this;
    }
    ScopedCustomShader2D FlipPageShader::start() const
    {
        return m_pImpl->start();
    }
    void FlipPageShader::apply(std::function<void()> drawble) const
    {
        m_pImpl->apply(std::move(drawble));
    }
}