#pragma once

namespace ct
{
    struct StopRange
    {
        double m_judgeCount;
        double m_rangeCount;
        StopRange(double judgeCount, double rangeCount) :
            m_judgeCount(judgeCount),
            m_rangeCount(rangeCount)
        {}
    };

    struct StopInfo
    {
        double m_firstCount;//最終更新のあったストップのカウント
        double m_totalOffset; //今までのトータルの停止時間におるオフセットぶん
        StopInfo() :
            m_firstCount(0),
            m_totalOffset(0)
        {}
        StopInfo(double firstCount, double totalOffset) :
            m_firstCount(firstCount),
            m_totalOffset(totalOffset)
        {}

    };
}
