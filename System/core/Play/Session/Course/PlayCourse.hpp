#pragma once
#include <memory>
#include <Siv3D/Fwd.hpp>
#include <core/Play/Session/IPlaySession.hpp>

namespace  ct
{
    struct CourseScore;
    class CourseData;
    class NotesData;

    class PlayCourse : public IPlaySession
    {
    public:
        enum State :s3d::uint8
        {
            None = 0,
            Playing = 1,
            Success = 2,
            Failure = 4,
            End = Success | Failure,
        };
    public:
        PlayCourse(const CourseData& course, LifeGaugeKind gauge);
    public:
        void next() override;
        bool isEnd() const override;

        s3d::Optional<s3d::String> playlistName() const override;
        LifeGaugeKind gauge() const;

        const NotesData& getNotes() const override;
        bool canForceFinishMusicGame() const override;

        const s3d::Array<PlayTrack>& playlist() const override;
        size_t getTrackIndex() const override;

        const PlayingScore& getScore() const override;
        bool keepScore() const override;
        void updateScore(const PlayingScore& score) override;

        const ScoreModel& getResult() const override;
        const CourseResult* getCourseResult() const override;
        const EndlessResult* getEndlessResult() const override
        {
            return nullptr;
        }
        s3d::StringView selectScene() const override;
        s3d::StringView playlistScene() const override;
    public:
        const CourseData& getCourse() const;
    private:
        class Impl;
        std::shared_ptr<Impl> m_pImpl;
    };
}