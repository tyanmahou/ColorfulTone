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

        /// <summary>
        /// ShiftJIS To UTF-8
        /// </summary>
        /// <param name="filePath"></param>
        /// <returns></returns>
        static bool ConvertShiftJISToUTF8(s3d::FilePathView filePath);

        /// <summary>
        /// UTF-8に変換
        /// </summary>
        /// <param name="filePath"></param>
        /// <returns></returns>
        static bool ConverToUTF8(s3d::FilePathView filePath);
    };
}