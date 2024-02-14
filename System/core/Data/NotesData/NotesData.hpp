#pragma once
#include <core/Data/Score/Score.hpp>
#include <core/Data/NotesData/SheetMusic.hpp>

namespace ct
{
    class MusicData;
    class MusicHandle;


    class NotesData
    {
        class Handle;
    public:
        //1小節のカウント数
        static constexpr s3d::int32 RESOLUTION = SheetMusic::RESOLUTION;
    public:
        NotesData() = default;

        NotesData(
            const std::shared_ptr<MusicHandle>& pMusic,
            const s3d::String& dirPath,
            const s3d::String& filePath,
            size_t index
        );

        /// <summary>
        /// インデックス取得
        /// </summary>
        /// <returns></returns>
        size_t getIndex() const;

        /// <summary>
        /// レベル取得
        /// </summary>
        s3d::int32 getLevel() const;

        /// <summary>
        /// レベル名取得
        /// </summary>
        const s3d::String& getLevelName() const;

        /// <summary>
        /// ★レベル
        /// </summary>
        /// <returns></returns>
        StarLv getStarLv() const;

        /// <summary>
        /// ★付きレベル名取得
        /// </summary>
        /// <returns></returns>
        s3d::String getLevelWithStar()const;

        /// <summary>
        /// レベル名とレベル取得
        /// </summary>
        /// <returns></returns>
        const s3d::String getLevelNameAndLevel() const;

        /// <summary>
        /// 譜面作成者取得
        /// </summary>
        const s3d::String& getNotesArtistName()const;

        /// <summary>
        /// ファイル名取得
        /// </summary>
        const s3d::String& getFileName()const;

        /// <summary>
        /// ノーツ数取得
        /// </summary>
        s3d::uint32 getTotalNotes() const;

        /// <summary>
        /// 譜面カラー
        /// </summary>
        const s3d::Color& getColor()const;

        /// <summary>
        /// 楽譜取得
        /// </summary>
        const SheetMusic& getSheet() const;

        const ScoreModel& getScore()const;
        void setScore(const ScoreModel& newScore);
        void saveScore(const ScoreModel& score) const;

        MusicData getMusic() const;
    private:
        std::shared_ptr<Handle> m_handle;
    };

}
