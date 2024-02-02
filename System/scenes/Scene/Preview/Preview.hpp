#pragma once
#include <memory>

namespace ct
{
    /// <summary>
    /// プレビュー
    /// </summary>
    class Preview
    {
    public:
        Preview();
        ~Preview();

        void init();
        bool updateAndDraw();
    private:
        class Impl;
        std::unique_ptr<Impl> m_pImpl;
    };
}