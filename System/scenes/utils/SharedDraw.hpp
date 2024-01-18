#pragma once
#include <scenes/utils/SharedDraw/DrawBGLight.hpp>
#include <scenes/utils/SharedDraw/LoadingProgress.hpp>

namespace ct::SharedDraw
{
    /// <summary>
    /// 付箋タイトル
    /// </summary>
    /// <param name="blueText"></param>
    /// <param name="redText"></param>
    void Sticky(const s3d::String* blueText, const s3d::String* redText = nullptr);
}