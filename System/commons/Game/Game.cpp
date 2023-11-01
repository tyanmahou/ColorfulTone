#include <commons/Game/Game.hpp>
#include <commons/Game/GameConfig.hpp>
#include <apis/Title/TitleApi.hpp>

namespace ct
{
    class Game::Impl
    {
    public:
        //Array<MusicData> m_musics;
        //Array<CourseData> m_courses;
        //Array<TapSE> m_tapSEs;

        GameConfig m_config;
    };
    Game::Game() :
        m_pImpl(std::make_unique<Impl>())
    {
    }
    Game::~Game()
    {
    }

    void Game::Startup()
    {
        TitleApi::CheckVersion();

        //ゲームコンフィグの初期化
        //キー入力やハイスピ等
        Config().init();
    }

    bool Game::Update()
    {
        return true;
    }
    void Game::Shutdown()
    {
        Config().save();
    }
    //Array<MusicData>& Game::Musics()
    //{
    //    return Instance()->m_pImpl->m_musics;
    //}
    //Array<CourseData>& Game::Courses()
    //{
    //    return Instance()->m_pImpl->m_courses;
    //}
    //Array<TapSE>& Game::TapSEs()
    //{
    //    return Instance()->m_pImpl->m_tapSEs;
    //}
    GameConfig& Game::Config()
    {
        return Instance()->m_pImpl->m_config;
    }
}
