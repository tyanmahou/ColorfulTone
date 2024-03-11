#pragma once
#include <Siv3D/Fwd.hpp>
#include <core/Types.hpp>
#include <core/Judge/Judge.hpp>
#include <core/Object/ObjectSorter.hpp>

namespace ct
{
    struct StopRange;

    /// <summary>
    /// 再生状態
    /// </summary>
    struct PlaybackState
    {
        s3d::int64 samplePos;
        BPMType bpm;
    };

    class Object
    {
    public:
        enum
        {
            RESOLUTION = 60,
        };	//間隔の調整
    protected:
        const s3d::int64 m_timingSample = 0;
        const double m_drawCountBase;
        double m_drawCount;

        bool m_isActive;			//存在フラグ
    public:

        Object() = default;
        Object(s3d::int64 timingSample, double firstCount) :
            m_timingSample(timingSample),
            m_drawCountBase(firstCount),
            m_drawCount(firstCount),
            m_isActive(true)
        {};
        virtual void init() { m_isActive = true; }
        virtual ~Object() {};
        virtual bool update(const PlaybackState& state) = 0;

        virtual void diffDraw(double count, double scrollRate)const = 0;

        void draw(double nowCount, double scrollRate)const;
        const double& getDrawCount()const { return m_drawCount; }
        double getFirstCount() const
        {
            return m_drawCountBase;
        }

        //譜面停止情報の調整
        void addStopCount(const StopRange& stopRange);

        void setActive(bool isActive)
        {
            m_isActive = isActive;
        }
        bool isActive() const
        {
            return m_isActive;
        }
        static bool CanDraw(const s3d::Vec2& pos);

        s3d::int64 getTimingSample() const
        {
            return m_timingSample;
        }
        s3d::int64 getTimingSampleOffset(s3d::int64 sample) const;
        bool isInJudgeRange(s3d::int64 sample, Judge judge) const;

        virtual ObjectOrder getOrder(s3d::int64 sample) const = 0;
    };
}
