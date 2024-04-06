#pragma once
#include <core/Data/NotesData/NotesData.hpp>
#include <core/SoundBeat.hpp>
#include <Siv3D/Texture.hpp>
#include <Siv3D/DateTime.hpp>
#include <Siv3D/AudioAsset.hpp>

namespace ct
{
    class MusicHandle;

    /// <summary>
    /// 楽曲データ
    /// </summary>
    class MusicData
    {
    public:
        /// <summary>
        /// インデックスの初期化
        /// </summary>
        static void ResetIndex();
    public:
        using ABLoop = s3d::Vec2;
    public:
        MusicData() = default;
        MusicData(const std::shared_ptr<MusicHandle>& handle);
        MusicData(const s3d::String& genreName, const s3d::String& dirPath, const s3d::String& iniPath, bool isOfficial);

        /// <summary>
        /// インデックス
        /// </summary>
        size_t getIndex() const;

        /// <summary>
        /// ジャケットテクスチャの取得
        /// </summary>
        const s3d::Texture& getTexture() const;

        /// <summary>
        /// サウンドデータの取得
        /// </summary>
        s3d::Audio getSound() const;

        /// <summary>
        /// 楽曲名の取得
        /// </summary>
        const s3d::String& getMusicName()const;

        /// <summary>
        /// アーティスト名の取得
        /// </summary>
        const s3d::String& getArtistName()const;

        /// <summary>
        /// 出典の取得
        /// </summary>
        const s3d::Optional<s3d::String>& getAuthority()const;

        /// <summary>
        /// ファイル名取得
        /// </summary>
        const s3d::String& getFileName()const;

        /// <summary>
        /// ジャンル名取得
        /// </summary>
        const s3d::String& getGenreName()const;

        /// <summary>
        /// ループ範囲取得
        /// </summary>
        const ABLoop& getLoopRange()const;

        /// <summary>
        /// 最終更新日取得
        /// </summary>
        const s3d::DateTime& getLastUpdateAt()const;

        /// <summary>
        /// アーティスト名と出典名を取得
        /// </summary>
        /// <returns></returns>
        const s3d::String getArtistAndAuthority() const;

        /// <summary>
        /// 最小BPMのビート
        /// </summary>
        SoundBar getMinSoundBeat()const;

        /// <summary>
        /// 最大BPMのビート
        /// </summary>
        /// <returns></returns>
        SoundBar getMaxSoundBeat()const;

        /// <summary>
        /// 最小BPM
        /// </summary>
        BPMType getMinBPM()const;

        /// <summary>
        /// 最大BPM
        /// </summary>
        BPMType getMaxBPM()const;

        /// <summary>
        /// 形式BPM
        /// </summary>
        s3d::String getFormattedBpm()const;

        /// <summary>
        /// サウンドID
        /// </summary>
        const s3d::String& getSoundNameID()const;

        // 曲の長さ
        const double getLengthSec()const;

        /// <summary>
        /// お気に入りか
        /// </summary>
        bool isFavorite()const;

        /// <summary>
        /// お気に入りパス
        /// </summary>
        /// <returns></returns>
        s3d::String getFavoriteFilePath()const;

        /// <summary>
        /// お気に入りの保存
        /// </summary>
        void saveFavorite(bool isFavorite);

        /// <summary>
        /// 公式楽曲か
        /// </summary>
        /// <returns></returns>
        bool isOfficial() const;

        /// <summary>
        /// 譜面データ取得
        /// </summary>
        s3d::Array<NotesData>& getNotesData();
        const s3d::Array<NotesData>& getNotesData()const;

        const NotesData& operator [](size_t level) const;
        NotesData& operator [](size_t level);

        explicit operator bool() const;

        [[nodiscard]] bool operator ==(const MusicData& other) const;
    private:
        std::shared_ptr<MusicHandle> m_handle;
    };
}
