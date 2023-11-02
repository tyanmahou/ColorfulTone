#pragma once
#include<Siv3D/Fwd.hpp>
#include<core/Types.hpp>

namespace ct
{
    struct StopRange;

    class Object
    {
    public:
        enum
        {
            RESOLUTION = 60,
        };	//間隔の調整
    protected:
        const double m_count;		//カウント
        double m_drawCount;

    public:
        bool m_isActive;			//存在フラグ

        Object() = default;
        Object(double firstCount) :
            m_count(firstCount),
            m_isActive(true),
            m_drawCount(firstCount)
        {};
        virtual void init() { m_isActive = true; }
        virtual ~Object() {};
        virtual bool update(double nowCount, double countPerFrame) = 0;

        virtual void diffDraw(double count, float scrollRate)const = 0;

        void draw(double nowCount, float scrollRate)const;
        const double& getCount()const { return m_count; }
        const double& getDrawCount()const { return m_drawCount; }

        //譜面停止情報の調整
        void addStopCount(const StopRange& stopRange);

        static bool CanDraw(const s3d::Vec2& pos);
    };
}
