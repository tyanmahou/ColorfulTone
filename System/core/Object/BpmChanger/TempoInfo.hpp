#pragma once
#include <core/SoundBeat.hpp>

namespace ct
{
    struct TempoInfo
    {
        s3d::int64 m_changeSample = 0;
        SoundBar m_bar;
        TempoInfo(s3d::int64 sample, s3d::int64 offsetSample, BPMType bpm) :
            m_changeSample(sample),
            m_bar(offsetSample, bpm)
        {}
    };
}