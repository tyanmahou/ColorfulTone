#pragma once
#include <memory>
#include <core/Play/Session/IPlaySession.hpp>

namespace ct
{
    class MusicData;

    class PlaySession
    {
    public:
        template<class T, class...Args>
        void start(Args&&... args)
        {
            m_impl = std::make_shared<T>(std::forward<Args>(args)...);
        }
        void exit();

        void next() const;
        bool isEnd() const;

        /// <summary>
        /// タイトル
        /// </summary>
        s3d::Optional<s3d::String> playlistName() const;

        /// <summary>
        /// ライフゲージ
        /// </summary>
        LifeGaugeKind gauge() const;

        /// <summary>
        /// 無敵か
        /// </summary>
        /// <returns></returns>
        bool isInvincible() const;
        /// <summary>
        /// 本合格が可能なゲージか
        /// </summary>
        bool isMainPassableGauge() const;

        const NotesData& getNotes() const;
        MusicData getMusic() const;

        bool canForceFinishMusicGame() const;
        bool canChangeGauge() const;

        const s3d::Array<PlayTrack> playlist() const;
        size_t getTrackIndex() const;
        size_t getTrackOrder() const;
        bool isFristTrack() const;

        const PlayingScore& getScore() const;
        bool keepScore() const;
        void updateScore(const PlayingScore& score) const;

        const ScoreModel& getResult() const;
        const CourseResult* getCourseResult() const;
        const EndlessResult* getEndlessResult() const;

        s3d::StringView selectScene() const;
        s3d::StringView playlistScene() const;

        template<class T>
        std::shared_ptr<T> cast()
        {
            return std::dynamic_pointer_cast<T>(m_impl);
        }
    private:
        std::shared_ptr<IPlaySession> m_impl;
    };
}