#include <core/Input/IGameInputPlayer.hpp>
#include <core/Input/PlayKey.hpp>
#include <Siv3D.hpp>

namespace ct
{
    const s3d::InputGroup& GetPlayKey(GameInputKind kind)
    {
        switch (kind) {
        case GameInputKind::Red:
            return PlayKey::Red();
        case GameInputKind::Blue:
            return PlayKey::Blue();
        case GameInputKind::Yellow:
            return PlayKey::Yellow();
        default:
            break;
        }
        return PlayKey::Red();
    }
}
