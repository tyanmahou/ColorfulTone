#pragma once
#include <memory>
#include <Siv3D/Fwd.hpp>
#include <core/Play/LifeGauge/LifeGaugeKind.hpp>

namespace  ct
{
    struct CourseScore;
    class CourseData;
    class NotesData;

    class PlayCourse
    {
    public:
        enum State :s3d::uint8
        {
            None = 0,
            Playing = 1,
            Success = 2,
            Failure = 4,
            End = 6,
        };
    public:
        PlayCourse();
        void init(const CourseData& course, LifeGaugeKind guage) const;
        void exit() const;

        bool isActive()const;

        size_t next() const;

        bool isStart() const;
        bool isEnd() const;
        bool isSuccess()const;
        bool isFailure()const;
        void updateScoreAndState(float addRate, float life)const;
        const CourseData& getCourse() const;
        const NotesData& getCurrentNotes() const;
        const s3d::Array<CourceSelectedNotes>& getSelectedNotes() const;
        size_t getTrackIndex()const;
        size_t getTrackOrder()const;
        State getState()const;

        const CourseScore& getScore() const;

        LifeGaugeKind getGaugeKind() const;
        bool isInvincible() const;
        /// <summary>
        /// 本合格が可能なゲージか
        /// </summary>
        /// <returns></returns>
        bool isMainPassableGauge() const;
    private:
        class Impl;
        std::shared_ptr<Impl> m_pImpl;
    };
}