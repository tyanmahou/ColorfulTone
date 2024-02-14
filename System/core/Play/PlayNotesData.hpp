#pragma once
#include <core/Object/Object.hpp>
#include <core/Object/BpmChanger/TempoInfo.hpp>
#include <core/Object/Stop/Stop.hpp>
#include <core/Data/Score/Score.hpp>

namespace ct
{
    class SheetMusic;

    class PlayNotesData
    {
    public:
        PlayNotesData();

        void init(const SheetMusic& sheet);
        void reset();
        void synchroCount(const s3d::Audio& sound, double& nowCount);	//カウントと同期

        void update(const s3d::Audio& sound, double& nowCount, Score& score);

        void draw(double nowCount, double scrollRate)const;
        void previewDraw(double nowCount, double scrollRate) const;

        double calDrawCount(const double currentCount)const;
    private:
        s3d::Array<std::shared_ptr<Object>> m_objects;	//譜面データ
        s3d::Array<StopRange> m_stopRanges;				//ストップのデータ
        s3d::Array<TempoInfo> m_tempoInfos;				//テンポ配列
        size_t m_currentBarIndex = 0;			        //現在のテンポ
    };
}