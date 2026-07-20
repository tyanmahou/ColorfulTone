#pragma once
#include <Siv3D/String.hpp>
#include <Siv3D/Array.hpp>
#include <core/Play/Session/PlayTrack.hpp>

namespace ct
{
    struct PlayingScore;

    struct ScoreModel;
    struct CourseResult;
    struct EndlessResult;

    class NotesData;

    class IPlaySession
    {
    public:
        IPlaySession() = default;
        virtual ~IPlaySession() = default;

        virtual void next() = 0;
        virtual bool isEnd() const = 0;

        /// <summary>
        /// プレイリストタイトル
        /// </summary>
        virtual s3d::Optional<s3d::String> playlistName() const = 0;

        /// <summary>
        /// ゲージ
        /// </summary>
        virtual LifeGaugeKind gauge() const = 0;

        /// <summary>
        /// 譜面
        /// </summary>
        virtual const NotesData& getNotes() const = 0;

        /// <summary>
        /// 楽曲を強制終了できるか
        /// </summary>
        virtual bool canForceFinishMusicGame() const = 0;

        /// <summary>
        /// プレイリスト
        /// </summary>
        virtual const s3d::Array<PlayTrack>& playlist() const = 0;

        /// <summary>
        /// Trackのインデックス
        /// </summary>
        virtual size_t getTrackIndex() const = 0;

        /// <summary>
        /// プレイスコア
        /// </summary>
        virtual const PlayingScore& getScore() const = 0;

        /// <summary>
        /// スコアを維持するか
        /// </summary>
        /// <returns></returns>
        virtual bool keepScore() const = 0;

        /// <summary>
        /// スコアを維持するか
        /// </summary>
        /// <returns></returns>
        virtual void updateScore(const PlayingScore& score) = 0;

        virtual const ScoreModel& getResult() const = 0;
        virtual const CourseResult* getCourseResult() const = 0;
        virtual const EndlessResult* getEndlessResult() const = 0;

        /// <summary>
        /// 選択画面
        /// </summary>
        virtual s3d::StringView selectScene() const = 0;
        virtual s3d::StringView playlistScene() const = 0;
    };
}