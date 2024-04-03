#pragma once
#include <Siv3D/Types.hpp>

namespace ct
{
    enum class LifeGaugeKind
    {
        Invincible  = -3,
        Safety      = -2,
        Support     = -1,
        Default     =  0,
        Challenge   =  1,
        Danger      =  2,
        SuddenDeath =  3,
    };
    struct LifeRecoverySet
    {
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
                .great = -50,
                .good = -400,
                .miss = -1000,
            };
        }
        static constexpr LifeRecoverySet Challenge()
        {
            return LifeRecoverySet{
                .perfect = 5,
                .great = -30,
                .good = -200,
                .miss = -750,
            };
        }
        static constexpr LifeRecoverySet Default()
        {
            return LifeRecoverySet{
                .perfect = 5,
                .great = -15,
                .good = -100,
                .miss = -500,
            };
        }
        static constexpr LifeRecoverySet Support()
        {
            return LifeRecoverySet{
                .perfect = 5,
                .great = -7,
                .good = -50,
                .miss = -250,
            };
        }
        static constexpr LifeRecoverySet Safety()
        {
            return LifeRecoverySet{
                .perfect = 10,
                .great = 1,
                .good = -25,
                .miss = -100,
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

    };
}