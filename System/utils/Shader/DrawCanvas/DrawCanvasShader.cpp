#include "DrawCanvasShader.hpp"
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
    class DrawCanvasShader::Impl
    {
    public:
        Impl() :
            m_ps(HLSL(Resource(U"Shaders/drawCanvas.ps"))),
            m_mask(U"Resource/Img/MusicSelect/maskDrawCanvas.png")
        {}
        void setTimer(double timer)
        {
            m_cb->timer = static_cast<float>(timer);
        }
        ScopedCustomShader2D start() const
        {
            Graphics2D::SetConstantBuffer(ShaderStage::Pixel, 1, m_cb);
            Graphics2D::SetPSTexture( 1, m_mask);
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
        Texture m_mask;
    };
    DrawCanvasShader::DrawCanvasShader() :
        m_pImpl(std::make_unique<Impl>())
    {
    }
    DrawCanvasShader::~DrawCanvasShader()
    {
    }
    const DrawCanvasShader& DrawCanvasShader::setTimer(double timer) const
    {
        m_pImpl->setTimer(timer);
        return *this;
    }
    s3d::ScopedCustomShader2D DrawCanvasShader::start() const
    {
        return m_pImpl->start();
    }
    void DrawCanvasShader::apply(std::function<void()> drawble) const
    {
        m_pImpl->apply(std::move(drawble));
    }
}
