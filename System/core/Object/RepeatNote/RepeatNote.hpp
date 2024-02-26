#pragma once
#include <core/Object/LongNote/LongNote.hpp>

namespace ct
{
    //連打ノーツ
    class RepeatNote :public Note
    {
    private:

        //連打開始
        bool m_isStart = false;

    public:
        RepeatNote(s3d::int64 timingSample, double firstCount, double speed) :
            Note(timingSample, 10, firstCount, speed)
        {}

        void init()override;

        bool update(const PlayContext& context)override;
        void diffDraw(double count, double scrollRate)const override;

        bool isFirstTap()const override
        {
            return m_isStart;
        }

        ObjectOrder getOrder(s3d::int64 sample) const override
        {
            return ObjectOrder::Calc(this, sample);
        }
    };

    //連打の終点
    class RepeatEnd :public LongNote
    {
    private:
        s3d::int64 m_lastSamplePos = 0;
        bool m_isStart = false;
        bool m_isTap = false;
        double m_interval;//連打間隔
    public:
        static s3d::int64 notesTapSample;

        RepeatEnd(s3d::int64 timingSample, double firstCount, double speed, std::shared_ptr<Note>& parent, double interval = 8.0);
        void init()override;
        bool update(const PlayContext& context)override;
        void diffDraw(double count, double scrollRate)const override;

        ObjectOrder getOrder(s3d::int64 sample) const override
        {
            return ObjectOrder::Calc(this, sample);
        }
    };
}
