#pragma once
#include <core/Object/Object.hpp>
#include <core/Object/BpmChanger/TempoInfo.hpp>
#include <core/Object/Stop/Stop.hpp>
#include <core/Data/NotesData/StarLv.hpp>
#include <core/Data/Score/Score.hpp>
#include <Siv3D/Color.hpp>
#include <Siv3D/CSV.hpp>

namespace ct
{
    class MusicData;
    class MusicHandle;


    class NotesData
    {
    public:
        static const s3d::int32 RESOLUTION = 9600;	    //1小節のカウント数

    private:
        s3d::int32 m_lv;								//レベル
        s3d::String m_lvName;							//レベル名
        s3d::String m_notesArtistName;					//譜面製作者の名前
        BPMType m_bpm;								    //最初のbpm
                                                        
        double m_maxBarCount;						    //小節数
        s3d::uint32 m_totalNotes;						//ノーツ数の合計
        s3d::int64 m_offsetSample;						//オフセットサンプル数
        s3d::Array<std::shared_ptr<Object>> m_objects;	//譜面データ
        s3d::Array<StopRange> m_stopRanges;				//ストップのデータ
        s3d::Array<TempoInfo> m_tempoInfos;				//テンポ配列
        size_t m_currentBarIndex = 0;			        //現在のテンポ

        s3d::String m_fileName;							//譜面ファイルの名前(拡張子を含まない)

        ScoreModel m_score;							    //スコア

        s3d::Color m_color;								//色
        StarLv m_starLv = StarLv::None;                 // ★レベル
        std::weak_ptr<MusicHandle> m_pMusic;					    //曲情報
        size_t m_index;							        //ID
    public:
        NotesData() = default;

        NotesData(
            const std::shared_ptr<MusicHandle>& pMusic,
            const s3d::String& dirPath,
            const s3d::String& filePath,
            size_t index
        );
        void init();
        void synchroCount(const s3d::Audio& sound, double& nowCount);	//カウントと同期

        void update(s3d::Audio& sound, double& nowCount, Score& score);
        void draw(double nowCount, double scrollRate)const;

        void previewDraw(double nowCount, double scrollRate) const;

        const double calDrawCount(const double currentCount)const
        {
            double drawCount = currentCount;
            for (auto&& elm : m_stopRanges) {
                if (currentCount >= elm.m_judgeCount) {
                    drawCount -= std::min(elm.m_rangeCount, currentCount - elm.m_judgeCount);
                }

            }
            return drawCount;
        }


        const s3d::int32& getLevel()const { return m_lv; }
        const double& getMaxBarCount()const { return m_maxBarCount; }
        const s3d::uint32& getTotalNotes()const { return m_totalNotes; }
        const s3d::String& getLevelName()const { return m_lvName; }
        s3d::String getLevelWithStar()const;
        const s3d::String getLevelNameAndLevel() const;
        const s3d::String& getNotesArtistName()const { return  m_notesArtistName; }
        const s3d::String& getFileName()const { return m_fileName; }
        const ScoreModel& getScore()const { return m_score; }
        void setScore(const ScoreModel newScore)
        {
            m_score = newScore;
        }

        BPMType getCurrentBPM()const
        {
            return m_tempoInfos[m_currentBarIndex].m_bar.getBPM();
        }

        const s3d::Color& getColor()const
        {
            return m_color;
        }
        StarLv getStarLv() const { return m_starLv; }

        MusicData getMusic() const;

        size_t getIndex()const
        {
            return m_index;
        }
        s3d::String getScorePath() const;
        void load(s3d::CSV& csv);

        void saveScore(const ScoreModel& score) const;
    };

}
