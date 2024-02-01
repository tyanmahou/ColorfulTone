#include <utils/Migration/Migration.hpp>
#include <commons/Migration/MigrationUtil.hpp>
#include <Siv3D.hpp>

namespace
{
    using namespace ct;

    MIGRATION(FixEncodeToUTF8)
    {
        void up() const override
        {
            // エンコード修正
            MigrationUtil::FixFileEncodeToUTF8();
        }
        void down() const override
        {
            // Not Impl
        }
    }_;
}
