#pragma once
#include <core/Data/NotesData/StarLv.hpp>

namespace ct::SharedDraw
{
    class WarningInfo
    {
    public:
        WarningInfo();

        const WarningInfo& setStarLv(StarLv starLv) const;
        void apply(std::function<void()> drawble) const;
    private:
        class Impl;
        std::shared_ptr<Impl> m_pImpl;
    };
}