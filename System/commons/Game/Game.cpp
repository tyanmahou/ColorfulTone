#include <commons/Game/Game.hpp>
#include <apis/Title/TitleApi.hpp>

namespace ct
{
    class Game::Impl
    {

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
    }

    bool Game::Update()
    {
        return true;
    }
    void Game::Shutdown()
    {
    }
}
