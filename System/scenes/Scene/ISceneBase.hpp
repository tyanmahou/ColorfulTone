#pragma once
#include <Siv3D/SceneManager.hpp>
#include <core/Play/Session/PlaySession.hpp>
#include <scenes/Scene/SceneName.hpp>

namespace ct
{
    //共有データ
    struct GameData
    {
        double m_scrollRate;
        PlaySession session;

        s3d::StringView m_fromScene;
        s3d::StringView m_toScene = U"load";
    };


    using MyApp = s3d::SceneManager<s3d::StringView, GameData>;

    class ISceneBase : public MyApp::Scene
    {
    public:
        using MyApp::Scene::Scene;
    protected:
        virtual void finally() {};
        void changeScene(const s3d::StringView& state, s3d::int32 transitionTimeMillisec = 1000, s3d::CrossFade crossFade = s3d::CrossFade::Yes)
        {
            GameData& data = this->getData();
            data.m_fromScene = std::move(data.m_toScene);
            data.m_toScene = state;
            this->finally();
            this->MyApp::Scene::changeScene(state, transitionTimeMillisec, crossFade);
        }
    };
}
