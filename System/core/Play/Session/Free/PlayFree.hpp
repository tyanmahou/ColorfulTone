#pragma once
#include <core/Play/Session/IPlaySession.hpp>
#include <core/Play/LifeGauge/LifeGaugeKind.hpp>

namespace ct
{
    class NotesData;

    class PlayFree : public IPlaySession
    {
    public:
        PlayFree(const NotesData& notes, LifeGaugeKind gauge);
        
        void next()  override;
        bool isEnd() const override;
        s3d::Optional<s3d::String> playlistName() const override
        {
            return s3d::none;
        }
        LifeGaugeKind gauge() const override;

        const NotesData& getNotes() const override;
        bool canForceFinishMusicGame() const override;

        const s3d::Array<PlayTrack>& playlist() const override;
        size_t getTrackIndex() const override;

        const PlayingScore& getScore() const override;
        bool keepScore() const override;
        void updateScore(const PlayingScore& score) override;

        const ScoreModel& getResult() const override;
        const CourseResult* getCourseResult() const override
        {
            return nullptr;
        }
        const EndlessResult* getEndlessResult() const override
        {
            return nullptr;
        }
        s3d::StringView selectScene() const override;
        s3d::StringView playlistScene() const override;
    private:
        class Impl;
        std::shared_ptr<Impl> m_pImpl;
    };
}