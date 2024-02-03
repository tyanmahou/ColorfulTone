#pragma once
#include <memory>
#include <Siv3D/ScopedCustomShader2D.hpp>

namespace ct
{
    class DrawCanvasShader
    {
        class Impl;
    public:
        DrawCanvasShader();
        ~DrawCanvasShader();
        const DrawCanvasShader& setTimer(double timer) const;
        s3d::ScopedCustomShader2D start() const;
        void apply(std::function<void()> drawble) const;
    private:
        std::unique_ptr<Impl> m_pImpl;
    };
}
