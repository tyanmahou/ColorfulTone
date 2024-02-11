#pragma once
#include <memory>
#include <Siv3D/ColorF.hpp>

namespace ct
{
    class BlendShader
    {
        class Impl;
    public:
        BlendShader(const s3d::Size& size);
        ~BlendShader();

        const BlendShader& setColor(const s3d::ColorF& color) const;

        void apply(std::function<void()> drawble) const;
    private:
        std::unique_ptr<Impl> m_pImpl;
    };
}