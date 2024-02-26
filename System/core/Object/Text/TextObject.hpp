#pragma once
#include <core/Object/Object.hpp>
#include <Siv3D/String.hpp>

namespace ct
{
    class TextObject :public Object
    {
    private:
        const s3d::String m_msg;	//描画するメッセージ
        const s3d::uint64 m_drawTimeSample;
        s3d::int64 m_beginTimeSample;
        bool m_isDraw;

    public:
        TextObject(s3d::int64 timingSample, double firstCount, const s3d::String& msg, const double timeSec) :
            Object(timingSample, firstCount),
            m_msg(msg),
            m_drawTimeSample(static_cast<s3d::uint64>(timeSec * 44100)),
            m_beginTimeSample(0),
            m_isDraw(false)
        {};
        virtual ~TextObject() = default;
        virtual bool update(const PlayContext& context)override;
        virtual void diffDraw(double count, double scrollRate)const override;
        void init()override;

        ObjectOrder getOrder(s3d::int64 sample) const override
        {
            return ObjectOrder::Calc(this, sample);
        }
    };
}
