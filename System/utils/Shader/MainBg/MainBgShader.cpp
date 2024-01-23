#include <utils/Shader/MainBg/MainBgShader.hpp>
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
    class MainBgShader::Impl
    {
    public:
        Impl() :
            m_ps(HLSL(Resource(U"Shaders/mainBg.ps")))
        {}
        void setTimer(double timer)
        {
            m_cb->timer = static_cast<float>(timer);
        }
        ScopedCustomShader2D start() const
        {
            Graphics2D::SetConstantBuffer(ShaderStage::Pixel, 1, m_cb);
            return ScopedCustomShader2D{ m_ps };
        }
        void apply(std::function<void()> drawble) const
        {
            auto scoped = this->start();
            drawble();
        }
    private:
        PixelShader m_ps;
        ConstantBuffer<ShaderParam> m_cb;
    };
    MainBgShader::MainBgShader() :
        m_pImpl(std::make_unique<Impl>())
    {
    }
    MainBgShader::~MainBgShader()
    {
    }
    const MainBgShader& MainBgShader::setTimer(double timer) const
    {
        m_pImpl->setTimer(timer);
        return *this;
    }
    s3d::ScopedCustomShader2D MainBgShader::start() const
    {
        return m_pImpl->start();
    }
    void MainBgShader::apply(std::function<void()> drawble) const
    {
        m_pImpl->apply(std::move(drawble));
    }
}
