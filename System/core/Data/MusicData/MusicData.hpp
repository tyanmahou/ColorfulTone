#pragma once
#include <core/Data/NotesData/NotesData.hpp>
#include <Siv3D/Texture.hpp>
#include <Siv3D/DateTime.hpp>
#include <Siv3D/AudioAsset.hpp>

namespace ct
{
    class MusicData
    {
    public:
        using ABLoop = s3d::Vec2;

        static size_t Index;
    private:

        s3d::String m_musicName;				//曲名
        s3d::String m_artistName;			//アーティスト名
        s3d::Optional<s3d::String> m_authority;	// 出典
        s3d::String m_genreName;				//ジャンル名
        s3d::Texture m_texture;				//ジャケ絵

        BPMType m_minbpm, m_maxbpm;	//bpm

        SoundBar m_minBar, m_maxBar;//bpmのbeat

        ABLoop m_loop;					//ループ範囲

        s3d::Array<NotesData> m_notesDatas;	//譜面情報

        s3d::String m_fileName;		//ファイルの名前

        s3d::String m_soundNameID;	//アセットへのアクセス
        s3d::DateTime m_lastUpdateAt;//更新日時
        bool m_isFavorite = false; // お気に入り
        bool m_isOfficial = false; // 公式譜面
        size_t m_index;

    public:
        MusicData() = default;
        MusicData(const s3d::String& genreName, const s3d::String& dirPath, const s3d::String& iniPath, bool isOfficial);

        MusicData(const MusicData&) = delete;
        MusicData(MusicData&& other) noexcept;
        MusicData& operator =(const MusicData&) = delete;

        const s3d::Texture& getTexture()const { return m_texture; }

        s3d::Audio getSound()const { return s3d::AudioAsset(m_soundNameID); }
        const s3d::String& getMusicName()const { return m_musicName; }
        const s3d::String& getArtistName()const { return m_artistName; }
        const s3d::Optional<s3d::String>& getAuthority()const { return m_authority; }
        const s3d::String& getFileName()const { return m_fileName; }
        const s3d::String& getGenreName()const { return m_genreName; }
        const ABLoop& getLoopRange()const { return m_loop; }
        const s3d::DateTime& getLastUpdateAt()const { return m_lastUpdateAt; }

        const s3d::String getArtistAndAuthority() const;

        s3d::int32 getBPM() const;

        const SoundBar& getMinSoundBeat()const { return m_minBar; }
        const SoundBar& getMaxSoundBeat()const { if (m_maxbpm == -1)return m_minBar; return m_maxBar; }
        const BPMType& getMinBPM()const { return m_minbpm; };
        const BPMType& getMaxBPM()const { if (m_maxbpm == -1)return m_minbpm; return m_maxbpm; };
        const s3d::String getFormattedBpm()const;

        s3d::Array<NotesData>& getNotesData() { return m_notesDatas; }
        const s3d::Array<NotesData>& getNotesData()const { return m_notesDatas; }

        const s3d::String& getSoundNameID()const { return m_soundNameID; }
        const size_t getIndex()const { return m_index; }

        const NotesData& operator [](size_t level) const
        {
            return m_notesDatas[level];
        }
        NotesData& operator [](size_t level)
        {
            return m_notesDatas[level];
        }

        // 曲の長さ
        const double getLengthSec()const;

        bool isFavorite()const;
        void setFavorite(bool isFavorite);
        s3d::String getFavoriteFilePath()const;

        void saveFavorite(bool isFavorite);

        bool isOfficial() const
        {
            return m_isOfficial;
        }
    private:
        MusicData& operator =(MusicData&& other) noexcept;
    };
}
