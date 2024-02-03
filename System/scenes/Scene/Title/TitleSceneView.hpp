#pragma once
#include <memory>

namespace ct
{
    class TitleScene;

    class TitleSceneView
    {
    public:
        TitleSceneView(const TitleScene* const scene);
        ~TitleSceneView();
        void draw()const;

    private:
        class Impl;
        std::unique_ptr<Impl> m_pImpl;
    };
}
