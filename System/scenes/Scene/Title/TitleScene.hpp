#pragma once
#include <scenes/Scene/ISceneBase.hpp>
#include <scenes/Scene/Title/TitleSceneView.hpp>

namespace ct
{
    class TitleScene :public ISceneBase
    {
    public:
        enum class Mode :unsigned int
        {
            GameStart,
            Course,
            KeyConfig,
            Download,
            Reload,
            Tutorial,
            Access,
            Licence,
            Exit,
        };
    private:
        TitleSceneView m_view;
        static Mode m_mode;

        void onEnterMode();
    public:


        TitleScene(const InitData& init);
        ~TitleScene() = default;

        void finally() override;

        void update() override;
        void draw() const override;
        void drawFadeIn(double t) const override;
        void drawFadeOut(double t) const override;
        static Mode GetMode();
    };
}
