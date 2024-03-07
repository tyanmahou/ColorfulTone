#pragma once
#if __has_include(<commons/Secret/Secret_Product.hpp>)
#include <commons/Secret/Secret_Product.hpp>
#else
#include <Siv3D/String.hpp>
namespace ct
{
    class Secret
    {
    public:
        // TwitterAPI使用できなくなった
        // static inline const s3d::String TwitterApiKey = U"";
        // static inline const s3d::String TwitterApiSecret = U"";
        static inline const s3d::String HOMEPAGE_URL = U"HOMEPAGE_URL";
        static inline const s3d::String API_ENDPOINT = U"API_ENDPOINT";
        static inline const s3d::String DOWNLOAD_URL = U"DOWNLOAD_URL";
        static inline const s3d::String RESOURCE_URL = U"RESOURCE_URL";
    };
}
#endif

