#pragma once
#include <Siv3D/AudioAsset.hpp>

namespace ct
{
    inline s3d::AudioLoopTiming MakeLoopTiming(double beginSec, double endSec, s3d::uint32 samplingRate = s3d::Wave::DefaultSampleRate)
    {
        return s3d::AudioLoopTiming(static_cast<s3d::uint64>(beginSec * samplingRate), static_cast<s3d::uint64>(endSec * samplingRate));
    }
    inline s3d::AudioLoopTiming MakeLoopTiming(const s3d::Duration& begin, const s3d::Duration& end, s3d::uint32 samplingRate = s3d::Wave::DefaultSampleRate)
    {
        return MakeLoopTiming(begin.count(), end.count(), samplingRate);
    }
}