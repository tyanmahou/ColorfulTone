#pragma once
#include <Siv3D/ScopedRenderTarget2D.hpp>
#include <Siv3D/Transformer2D.hpp>

namespace ct
{
    struct ScopedResetTransRT
    {
    public:
        ScopedResetTransRT(const s3d::RenderTexture& rt):
            m_rt(rt),
            m_t2d(s3d::Mat3x2::Identity(), s3d::Transformer2D::Target::SetLocal)
        {

        }
    private:
        s3d::ScopedRenderTarget2D m_rt;
        s3d::Transformer2D m_t2d;
    };
}