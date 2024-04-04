#pragma once
#include <Siv3D/Types.hpp>
#include <Siv3D/Texture.hpp>
#include <core/Play/LifeGauge/LifeGaugeKind.hpp>

namespace ct
{
    struct LifeRecoverySet
    {
        static constexpr LifeRecoverySet FromKind(LifeGaugeKind kind)
        {
            switch (kind) {
            case LifeGaugeKind::Invincible:
                return LifeRecoverySet::Invincible();
            case LifeGaugeKind::Safety:
                return LifeRecoverySet::Safety();
            case LifeGaugeKind::Support:
                return LifeRecoverySet::Support();
            case LifeGaugeKind::Normal:
                return LifeRecoverySet::Normal();
            case LifeGaugeKind::Challenge:
                return LifeRecoverySet::Challenge();
            case LifeGaugeKind::Danger:
                return LifeRecoverySet::Danger();
            case LifeGaugeKind::SuddenDeath:
                return LifeRecoverySet::SuddenDeath();
            default:
                return LifeRecoverySet::Normal();
            }
        }
        static constexpr LifeRecoverySet SuddenDeath()
        {
            return LifeRecoverySet{
                .perfect = 0,
                .great = -1000,
                .good = -3000,
                .miss = -10000,
            };
        }
        static constexpr LifeRecoverySet Danger()
        {
            return LifeRecoverySet{
                .perfect = 3,
                .great = -50,  // 17
                .good = -400,  // 134
                .miss = -1000, // 334
            };
        }
        static constexpr LifeRecoverySet Challenge()
        {
            return LifeRecoverySet{
                .perfect = 5,
                .great = -30, // 6
                .good = -200, // 40
                .miss = -750, // 150
            };
        }
        static constexpr LifeRecoverySet Normal()
        {
            return LifeRecoverySet{
                .perfect = 5,
                .great = -15,  // 3
                .good = -100,  // 20
                .miss = -500,  // 100
            };
        }
        static constexpr LifeRecoverySet Support()
        {
            return LifeRecoverySet{
                .perfect = 5,
                .great = -7,  // 2
                .good = -50,  // 10
                .miss = -250, // 50
            };
        }
        static constexpr LifeRecoverySet Safety()
        {
            return LifeRecoverySet{
                .perfect = 10,
                .great = 0,
                .good = -50,  //  5
                .miss = -200, // 20
            };
        }
        static constexpr LifeRecoverySet Invincible()
        {
            return LifeRecoverySet{
                .perfect = 0,
                .great = 0,
                .good = 0,
                .miss = 0,
            };
        }
        s3d::int32 perfect;
        s3d::int32 great;
        s3d::int32 good;
        s3d::int32 miss;
    };

    class LifeGauge
    {
    public:
        static s3d::Texture GetBadge(LifeGaugeKind kind);
        static s3d::Texture GetTile(LifeGaugeKind kind);
    };
}