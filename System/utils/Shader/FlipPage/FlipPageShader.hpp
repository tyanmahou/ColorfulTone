#pragma once
#include <memory>
#include <Siv3D/ScopedCustomShader2D.hpp>

namespace ct
{
    class FlipPageShader
    {
        class Impl;
    public:
        FlipPageShader(const s3d::Size& size);
        ~FlipPageShader();
        const FlipPageShader& setTimer(double timer) const;
        const FlipPageShader& setIn(bool in) const;

        s3d::ScopedCustomShader2D start() const;
        void apply(std::function<void()> drawble) const;
    private:
        std::unique_ptr<Impl> m_pImpl;
    };
}
