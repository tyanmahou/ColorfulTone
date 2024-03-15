#pragma once
#include <scenes/utils/SharedDraw/DrawBGLight.hpp>
#include <scenes/utils/SharedDraw/LoadingProgress.hpp>
#include <scenes/utils/SharedDraw/LoadingCircle.hpp>
#include <scenes/utils/SharedDraw/JacketInfo.hpp>
#include <scenes/utils/SharedDraw/MemoInfo.hpp>
#include <scenes/utils/SharedDraw/Select.hpp>

namespace ct
{
    class HighSpeedDemo;
    class MusicData;
    class NotesData;
    class PlayNotesData;
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
    void HighSpeed(const HighSpeedDemo& highSpeedDemo, const MusicData& music, double scrollRate, bool canDemo = true);

    /// <summary>
    /// ハイスピード表示(プレイ用)
    /// </summary>
    /// <param name="highSpeedDemo"></param>
    /// <param name="music"></param>
    /// <param name="scrollRate"></param>
    /// <param name="canDemo"></param>
    void HighSpeedPlay(const HighSpeedDemo& highSpeedDemo, const PlayNotesData& noteData, double scrollRate, bool canDemo = true);

    /// <summary>
    /// 長押しバック
    /// </summary>
    /// <param name="input"></param>
    /// <param name="text"></param>
    void LongPressBack(const s3d::InputGroup& input, const s3d::String& text);

    /// <summary>
    /// ヘッダー描画
    /// </summary>
    /// <param name="pos"></param>
    void DrawPlayContextHeader(const s3d::Vec2& pos = s3d::Vec2{ s3d::Scene::CenterF().x, 40 });

    /// <summary>
    /// プレイスタイルアイコン
    /// </summary>
    void DrawPlayStyleIcon();
    void DrawPlayStyleIcon(const s3d::Vec2& pos);
}