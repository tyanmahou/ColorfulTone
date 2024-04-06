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
                .great = -1000, // 10
                .good = -3000,  // 4
                .miss = -10000, // 1
            };
        }
        static constexpr LifeRecoverySet Danger()
        {
            return LifeRecoverySet{
                .perfect = 2,
                .great = -500, // 20, 250
                .good = -1000, // 10, 500
                .miss = -2500, // 4, 1250
            };
        }
        static constexpr LifeRecoverySet Challenge()
        {
            return LifeRecoverySet{
                .perfect = 5,
                .great = -100, // 100,  20
                .good = -500,  // 20,  100
                .miss = -1000, // 10,  200
            };
        }
        static constexpr LifeRecoverySet Normal()
        {
            return LifeRecoverySet{
                .perfect = 5,
                .great = -15,  // 667,  3
                .good = -100,  // 100, 20
                .miss = -500,  // 20, 100
            };
        }
        static constexpr LifeRecoverySet Support()
        {
            return LifeRecoverySet{
                .perfect = 5,
                .great = -10, // 1000,  2
                .good =  -80, // 125,  16
                .miss = -250, // 40,   50
            };
        }
        static constexpr LifeRecoverySet Safety()
        {
            return LifeRecoverySet{
                .perfect = 10,
                .great = 0,
                .good = -50,  // 200, 5
                .miss = -200, // 50, 20
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