#include <core/Input/InputManager.hpp>
#include <core/Input/Auto/AutoPlayer.hpp>
#include <core/Input/FpsSync/FpsSyncPlayer.hpp>
#include <Siv3D.hpp>

namespace
{
    enum PlayerKind
    {
        FpsSync,
        AutoPlay,
    };
}
namespace ct
{
    InputManager::InputManager()
    {
        m_players << std::make_unique<FpsSyncPlayer>();
        m_players << std::make_unique<AutoPlayer>();
    }

    IGameInputPlayer* InputManager::getPlayer() const
    {
        if (PlayContext::IsAutoPlay()) {
            return m_players[AutoPlay].get();
        }
        return m_players[FpsSync].get();
    }

    bool InputManager::IsHighPrecision()
    {
        // ひとまずfalse
        return false;
    }

    bool InputManager::IsRedClicked()
    {
        return Instance()->getPlayer()->isClicked(GameInputKind::Red);
    }

    bool InputManager::IsRedPressed()
    {
        return Instance()->getPlayer()->isPressed(GameInputKind::Red);
    }

    void InputManager::PopRed()
    {
        Instance()->getPlayer()->pop(GameInputKind::Red);
    }

    s3d::int64 InputManager::RedClickedTimeOffset()
    {
        return Instance()->getPlayer()->clickedMillisecOffset(GameInputKind::Red);
    }

    bool InputManager::IsBlueClicked()
    {
        return Instance()->getPlayer()->isClicked(GameInputKind::Blue);
    }

    bool InputManager::IsBluePressed()
    {
        return Instance()->getPlayer()->isPressed(GameInputKind::Blue);
    }

    void InputManager::PopBlue()
    {
        Instance()->getPlayer()->pop(GameInputKind::Blue);
    }

    s3d::int64 InputManager::BlueClickedTimeOffset()
    {
        return Instance()->getPlayer()->clickedMillisecOffset(GameInputKind::Blue);
    }

    bool InputManager::IsYellowClicked()
    {
        return Instance()->getPlayer()->isClicked(GameInputKind::Yellow);
    }

    bool InputManager::IsYellowPressed()
    {
        return Instance()->getPlayer()->isPressed(GameInputKind::Yellow);
    }

    void InputManager::PopYellow()
    {
        Instance()->getPlayer()->pop(GameInputKind::Yellow);
    }

    s3d::int64 InputManager::YellowClickedTimeOffset()
    {
        return Instance()->getPlayer()->clickedMillisecOffset(GameInputKind::Yellow);
    }

    bool InputManager::IsAnyClicked()
    {
        return IsRedClicked() || IsBlueClicked() || IsYellowClicked();
    }

    void InputManager::Update()
    {
        Instance()->getPlayer()->update();
    }
    void InputManager::ForceInput(NoteType type)
    {
        auto* player = Instance()->getPlayer();
        const bool isLong = type >= 11 && type <= 17;
        switch (type) {
        case 1:
        case 11:
            player->forceInput(GameInputKind::Red, isLong);
            break;
        case 2:
        case 12:
            player->forceInput(GameInputKind::Blue, isLong);
            break;
        case 3:
        case 13:
            player->forceInput(GameInputKind::Yellow, isLong);
            break;
        case 4:
        case 14:
            player->forceInput(GameInputKind::Blue, isLong);
            player->forceInput(GameInputKind::Yellow, isLong);
            break;
        case 5:
        case 15:
            player->forceInput(GameInputKind::Red, isLong);
            player->forceInput(GameInputKind::Yellow, isLong);
            break;
        case 6:
        case 16:
            player->forceInput(GameInputKind::Red, isLong);
            player->forceInput(GameInputKind::Blue, isLong);
            break;
        case 7:
        case 17:
            player->forceInput(GameInputKind::Red, isLong);
            player->forceInput(GameInputKind::Blue, isLong);
            player->forceInput(GameInputKind::Yellow, isLong);
            break;
        }
    }
}
