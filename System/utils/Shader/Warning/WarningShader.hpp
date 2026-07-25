#pragma once
#include <memory>
#include <Siv3D/ColorF.hpp>

namespace ct
{
    class WarningShader
    {
        class Impl;
    public:
        WarningShader(const s3d::Size& size);
        ~WarningShader();

        const WarningShader& setColor(const s3d::ColorF& color) const;
        const WarningShader& setNoiseRate(double noiseRate) const;
        const WarningShader& setBlend(double blend) const;
        const WarningShader& setTime(double time) const;

        void apply(std::function<void()> drawable) const;
    private:
        std::unique_ptr<Impl> m_pImpl;
    };
}