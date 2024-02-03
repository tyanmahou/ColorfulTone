#include <utils/Migration/Migration.hpp>
#include <commons/Migration/MigrationUtil.hpp>
#include <Siv3D.hpp>

namespace
{
    using namespace ct;

    MIGRATION(MoveUserDataPath)
    {
        void up() const override
        {
            // スコアの保存場所を変更
            MigrationUtil::MoveScoreFolder();
        }
        void down() const override
        {
            // Not Impl
        }
    }_;
}
