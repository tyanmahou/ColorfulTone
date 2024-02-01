#pragma once
#include <Siv3D/Array.hpp>
#include <Siv3D/StringView.hpp>

namespace ct
{
    class MigrationUtil
    {
    public:
        /// <summary>
        /// UTF8エンコード
        /// </summary>
        static s3d::Array<s3d::FilePath> FixFileEncodeToUTF8();

        /// <summary>
        /// スコアのフォルダを移動する
        /// </summary>
        static void MoveScoreFolder();


        static void RenameMusicFolder(FilePathView from, FilePathView to);
    };
}