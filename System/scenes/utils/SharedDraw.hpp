#pragma once
#include <scenes/utils/SharedDraw/DrawBGLight.hpp>
#include <scenes/utils/SharedDraw/LoadingProgress.hpp>
#include <scenes/utils/SharedDraw/JacketInfo.hpp>
#include <scenes/utils/SharedDraw/MemoInfo.hpp>
#include <scenes/utils/SharedDraw/Select.hpp>

namespace ct
{
    class HighSpeedDemo;
    class MusicData;
}
namespace ct::SharedDraw
{
    /// <summary>
    /// 付箋タイトル
    /// </summary>
    /// <param name="blueText"></param>
    /// <param name="redText"></param>
    void Sticky(const s3d::String* blueText, const s3d::String* redText = nullptr);

    /// <summary>
    /// ハイスピード表示
    /// </summary>
    /// <param name="highSpeedDemo"></param>
    /// <param name="music"></param>
    /// <param name="scrollRate"></param>
    /// <param name="canDemo"></param>
    void HighSpeed(const HighSpeedDemo& highSpeedDemo, const MusicData& music, double scrollRate, bool canDemo);
}