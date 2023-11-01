#pragma once
#if __has_include(<commons/Secret/Secret_Product.hpp>)
#include<commons/Secret/Secret_Product.hpp>
#else
#include<Siv3D/String.hpp>
namespace ct
{
    class Secret
    {
    public:
        // TwitterAPI使用できなくなった
        // static inline const s3d::String TwitterApiKey = L"";
        // static inline const s3d::String TwitterApiSecret = L"";
        static inline const s3d::String HOMEPAGE_URL = L"HOMEPAGE_URL";
        static inline const s3d::String API_ENDPOINT = L"API_ENDPOINT";
        static inline const s3d::String DOWNLOAD_URL = L"DOWNLOAD_URL";
    };
}
#endif

