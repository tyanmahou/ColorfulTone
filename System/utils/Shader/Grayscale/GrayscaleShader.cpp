#include <utils/Shader/Grayscale/GrayscaleShader.hpp>
#include <Siv3D.hpp>

namespace ct
{
    class GrayscaleShader::Impl
    {
    public:
        Impl() :
            m_ps(HLSL(U"Shaders/grayscale.ps"))
        {
        }
        [[nodiscard]] s3d::ScopedCustomShader2D start() const
        {
            return s3d::ScopedCustomShader2D{ m_ps };
        }
    private:
        PixelShader m_ps;
    };
    GrayscaleShader::GrayscaleShader():
        m_pImpl(std::make_unique<Impl>())
    {
    }
    GrayscaleShader::~GrayscaleShader()
    {
    }
    s3d::ScopedCustomShader2D GrayscaleShader::start() const
    {
        return m_pImpl->start();
    }
}
