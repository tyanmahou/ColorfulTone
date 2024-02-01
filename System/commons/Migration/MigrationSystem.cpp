#include <commons/Migration/MigrationSystem.hpp>
#include <utils/Migration/Migration.hpp>
#include <Siv3D.hpp>

namespace ct
{
    using Migration::MigrationHundler;
    bool MigrationSystem::Up()
    {
        constexpr FilePathView path = U"UserData/System/.migration";

        Array<int64> versions;

        // 最新バージョンを調べる
        {
            JSON file = JSON::Load(path);
            if (file && file[U"up_versions"].isArray()) {
                for (JSON v : file[U"up_versions"].arrayView()) {
                    if (v.isInteger()) {
                        versions.push_back(v.get<int64>());
                    }
                }
            }
        }
        int64 currentVersion = versions.isEmpty() ? 0 : versions.back();

        s3d::Array<int64> updateVersions = MigrationHundler::UpdateVersions(currentVersion);
        if (updateVersions.empty()) {
            // 更新無し
            return false;
        }
        for (auto version : updateVersions) {
            MigrationHundler::Get(version)->up();
        }
        versions.append(std::move(updateVersions));
        {
            JSON json;
            json[U"up_versions"] = versions;
            json.save(path);
        }
        return true;
    }
}
