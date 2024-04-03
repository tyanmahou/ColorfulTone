#include <core/Play/LifeGauge/LifeGauge.hpp>
#include <Siv3D.hpp>

namespace ct
{
    Texture LifeGauge::GetBadge(LifeGaugeKind kind)
    {
        switch (kind) {
        case LifeGaugeKind::Invincible:
            return TextureAsset(U"IV_badge");
        case LifeGaugeKind::Safety:
            return TextureAsset(U"SF_badge");
        case LifeGaugeKind::Support:
            return TextureAsset(U"SP_badge");
        case LifeGaugeKind::Normal:
            return TextureAsset(U"NM_badge");
        case LifeGaugeKind::Challenge:
            return TextureAsset(U"CH_badge");
        case LifeGaugeKind::Danger:
            return TextureAsset(U"DG_badge");
        case LifeGaugeKind::SuddenDeath:
            return TextureAsset(U"SD_badge");
        default:
            break;
        }
        return TextureAsset(U"NM_badge");
    }
}
