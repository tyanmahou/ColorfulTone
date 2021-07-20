#pragma once
#include"Object.h"
#include"BPMChanger.h"
#include"Stop.h"
#include"Score.h"
#include "StarLv.hpp"

class MusicData;


class NotesData
{
public:
	static const int RESOLUTION = 9600;			//1小節のカウント数

private:
	int m_lv;									//レベル
	String m_lvName;							//レベル名
	String m_notesArtistName;					//譜面製作者の名前
	BPMType m_bpm;								//最初のbpm

	double m_maxBarCount;						//小節数
	int m_totalNotes;							//ノーツ数の合計
	int m_offsetSample;							//オフセットサンプル数
	Array<std::shared_ptr<Object>> m_objects;	//譜面データ
	Array<StopRange> m_stopRanges;				//ストップのデータ
	Array<TempoInfo> m_tempoInfos;				//テンポ配列
	unsigned int m_currentBarIndex=0;			//現在のテンポ

	String m_fileName;							//譜面ファイルの名前(拡張子を含まない)

	ScoreModel m_score;							//スコア

	Color m_color;								//色
	StarLv m_starLv = StarLv::None;             // ★レベル
	const MusicData* m_pMusic;					//曲情報
	uint32 m_index;								//ID
public:
	NotesData() = default;

	NotesData(
		const MusicData*const pMusic, 
		const String& dirPath, 
		const String& filePath,
		uint32 index
	);

	void init();
	void synchroCount(const Sound& sound, double& nowCount);	//カウントと同期

	void update(Sound& sound, double& nowCount, Score& score);
	void draw(const double& nowCount, float scrollRate)const;

	void previewDraw(const double & nowCount, float scrollRate) const;

	const double calDrawCount(const double currentCount)const
	{
		double drawCount = currentCount;
		for (auto&& elm : m_stopRanges)
		{
			if (currentCount >= elm.m_judgeCount)
			{
				drawCount -= std::min(elm.m_rangeCount, currentCount - elm.m_judgeCount);
			}

		}
		return drawCount;
	}


	const int& getLevel()const { return m_lv; }
	const double& getMaxBarCount()const { return m_maxBarCount; }
	const int& getTotalNotes()const { return m_totalNotes; }
	const String& getLevelName()const { return m_lvName; }
	String getLevelWithStar()const;
	const String getLevelNameAndLevel() const;
	const String& getNotesArtistName()const { return  m_notesArtistName; }
	const String& getFileName()const { return m_fileName; }
	const ScoreModel& getScore()const { return m_score; }
	void setScore(const ScoreModel newScore)
	{
		m_score = newScore;
	}

	BPMType getCurrentBPM()const
	{
		return m_tempoInfos[m_currentBarIndex].m_bar.getBPM();
	}

	const Color& getColor()const
	{
		return m_color;
	}
	StarLv getStarLv() const { return m_starLv; }

	const MusicData* const getMusic()const
	{
		return m_pMusic;
	}
	uint32 getIndex()const
	{
		return m_index;
	}
	String getScorePath() const;
	void load(CSVReader& csv);

	void saveScore(const ScoreModel& score) const;
};
