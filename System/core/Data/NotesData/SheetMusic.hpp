#pragma once
#include <utility>
#include <Siv3D/Array.hpp>
#include <Siv3D/String.hpp>
#include <Siv3D/Color.hpp>
#include <core/Types.hpp>
#include <core/Data/NotesData/StarLv.hpp>

namespace ct
{
    /// <summary>
    /// 音符
    /// </summary>
    struct NoteEntity
    {
        s3d::int64 sample;
        double count;

        NoteType type;
        double speed;

        double interval;
    };

    /// <summary>
    /// 小節
    /// </summary>
    struct BarEntity
    {
        s3d::int64 sample;
        double count;
        double speed;
    };

    /// <summary>
    /// 歌詞
    /// </summary>
    struct LyricEntity
    {
        s3d::int64 sample;
        double count;

        s3d::String message;
        double timeSec;
    };

    /// <summary>
    /// テンポ
    /// </summary>
    struct TempoEntity
    {
        s3d::int64 sample;
        double count;

        BPMType bpm;
        s3d::int64 bpmOffsetSample;
    };

    /// <summary>
    /// 停止
    /// </summary>
    struct StopEntity
    {
        double count;
        double rangeCount;
    };

    /// <summary>
    /// 楽譜
    /// </summary>
    class SheetMusic
    {
    public:
        // 1小節のカウント数
        static constexpr s3d::int32 RESOLUTION = 9600;
    public:
        SheetMusic();

        SheetMusic(s3d::FilePathView path);

        bool load(s3d::FilePathView path);

        s3d::int32 getLv() const
        {
            return m_lv;
        }
        const s3d::String& getLevelName() const
        {
            return m_lvName;
        }
        StarLv getStarLv() const
        {
            return m_starLv;
        }
        const s3d::String& getNotesArtistName() const
        {
            return m_notesArtistName;
        }
        const s3d::Color& getColor() const
        {
            return m_color;
        }
        const s3d::Array<NoteEntity>& getNotes() const
        {
            return m_notes;
        }
        const s3d::Array<BarEntity>& getBars() const
        {
            return m_bars;
        }
        const s3d::Array<LyricEntity>& getLyrics() const
        {
            return m_lyrics;
        }
        const s3d::Array<StopEntity>& getStops() const
        {
            return m_stops;
        }
        const s3d::Array<TempoEntity>& getTempos() const
        {
            return m_tempos;
        }
        double  getLastBarCount() const
        {
            return m_lastBarCount;
        }
        s3d::int64 getTotalSample() const
        {
            return m_totalSample;
        }
        double getTotalSec() const
        {
            return static_cast<double>(m_totalSample) / 44100.0;
        }
        s3d::uint32 getTotalNotes() const
        {
            return m_totalNotes;
        }
        s3d::String getLevelWithStar() const;

        s3d::String getLevelNameAndLevel() const;
    private:
        //** メタ情報 **//
        s3d::int32 m_lv;                  // レベル
        s3d::String m_lvName;             // レベル名
        StarLv m_starLv = StarLv::None;   // ★レベル
        s3d::String m_notesArtistName;    // 譜面製作者の名前

        s3d::Color m_color;               // 譜面カラー

        double m_lastBarCount;			  // 小節のカウント数
        s3d::int64 m_totalSample;		  // トータルサンプル
        s3d::uint32 m_totalNotes;		  // ノーツ数の合計
        //** 楽譜情報 **//
        s3d::int64 m_offsetSample;		  // オフセットサンプル数
        s3d::Array<NoteEntity> m_notes;   // 音符情報
        s3d::Array<BarEntity> m_bars;     // 小節情報
        s3d::Array<LyricEntity> m_lyrics; // 歌詞情報
        s3d::Array<StopEntity> m_stops;	  // ストップのデータ
        s3d::Array<TempoEntity> m_tempos; // テンポ配列
    };
}