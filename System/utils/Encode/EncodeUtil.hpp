#pragma once
#include <Siv3D/String.hpp>

namespace ct
{
    class EncodeUtil
    {
    public:
        /// <summary>
        /// Shift-JISか
        /// </summary>
        /// <param name="filePath"></param>
        /// <returns></returns>
        static bool IsMaybeShiftJIS(s3d::FilePathView filePath);

        static bool ConvertShiftJISToUTF8(s3d::FilePathView filePath);
    };
}