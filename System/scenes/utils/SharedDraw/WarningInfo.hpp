#pragma once
#include <core/Data/NotesData/StarLv.hpp>

namespace ct::SharedDraw
{
    class WarningInfo
    {
    public:
        WarningInfo();

        const WarningInfo& setStarLv(StarLv starLv) const;
        const WarningInfo& setColor(const s3d::Optional<s3d::Color>& color) const;
        void apply(std::function<void()> drawble) const;
    private:
        class Impl;
        std::shared_ptr<Impl> m_pImpl;
    };
}