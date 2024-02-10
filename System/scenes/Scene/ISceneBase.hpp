#pragma once
#include <Siv3D/SceneManager.hpp>
#include <core/Data/NotesData/NotesData.hpp>
#include <core/Play/PlayCourse.hpp>

namespace ct
{
    //共有データ
    struct GameData
    {
        NotesData m_nowNotes;

        double m_scrollRate;
        Score m_resultScore;

        PlayCourse m_course;

        s3d::String m_fromScene;
        s3d::String m_toScene = U"load";
    };


    using MyApp = s3d::SceneManager<s3d::String, GameData>;

    class ISceneBase : public MyApp::Scene
    {
    public:
        using MyApp::Scene::Scene;
    protected:
        virtual void finally() {};
        void changeScene(const s3d::String& state, s3d::int32 transitionTimeMillisec = 1000, s3d::CrossFade crossFade = s3d::CrossFade::Yes)
        {
            GameData& data = this->getData();
            data.m_fromScene = std::move(data.m_toScene);
            data.m_toScene = state;
            this->finally();
            this->MyApp::Scene::changeScene(state, transitionTimeMillisec, crossFade);
        }
    };

    namespace SceneName
    {
        constexpr auto Load = U"load";
        constexpr auto Title = U"title";
        constexpr auto Select = U"select";
        constexpr auto Main = U"main";
        constexpr auto Result = U"result";
        constexpr auto CourseSelect = U"courseSelect";
        constexpr auto Course = U"course";
        constexpr auto Config = U"config";
        constexpr auto Tutorial = U"tutorial";
        constexpr auto Download = U"download";
    }
}
