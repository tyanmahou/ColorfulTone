#pragma once
#include <Siv3D/Asset.hpp>

namespace ct
{
    struct SivAssetUtil
    {
        template<class AssetType>
        static void UnregisterByTag(const s3d::String& tag)
        {
            for (auto [name, info] : AssetType::Enumerate()) {
                if (info.tags.includes(tag)) {
                    AssetType::Unregister(name);
                }
            }
        }
        template<class AssetType>
        static void ReleaseByTag(const s3d::String& tag)
        {
            for (auto [name, info] : AssetType::Enumerate()) {
                if (info.tags.includes(tag)) {
                    AssetType::Release(name);
                }
            }
        }
    };
}