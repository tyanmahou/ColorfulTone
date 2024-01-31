#pragma once
#include <Siv3D/Fwd.hpp>
#include <core/Types.hpp>

namespace ct
{
    struct StopRange;

    struct PlayContext
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

    public:
        bool m_isActive;			//存在フラグ

        Object() = default;
        Object(s3d::int64 timingSample, double firstCount) :
            m_timingSample(timingSample),
            m_drawCountBase(firstCount),
            m_drawCount(firstCount),
            m_isActive(true)
        {};
        virtual void init() { m_isActive = true; }
        virtual ~Object() {};
        virtual bool update(const PlayContext& context) = 0;

        virtual void diffDraw(double count, double scrollRate)const = 0;

        void draw(double nowCount, double scrollRate)const;
        const double& getDrawCount()const { return m_drawCount; }

        //譜面停止情報の調整
        void addStopCount(const StopRange& stopRange);

        static bool CanDraw(const s3d::Vec2& pos);
    };
}
