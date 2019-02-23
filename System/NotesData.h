#pragma once
#include"Object.h"
#include"BPMChanger.h"
#include"Stop.h"
#include"Score.h"

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

	bool m_isClear;								//クリア情報
	float m_clearRate;							//達成率レコード
	SpecialResult m_specialResult;				//フルコン等の情報

	Color m_color;								//色
	MusicData* m_pMusic;						//曲情報
public:
	NotesData() = default;

	NotesData(
		const MusicData*const pMusic, 
		const String& genreName,
		const String& dirPath, 
		const String& filePath
	);

	void init();
	void synchroCount(const Sound& sound, double& nowCount);	//カウントと同期

	void update(Sound& sound, double& nowCount, Score& score);
	void draw(const double& nowCount, float scrollRate)const;

	void previewDraw(const double & nowCount, float scrollRate) const;

	//プロパティ使ってみたけど使う必要ないと思う
	__declspec(property(get = _getIsClear, put = _setIsClear))bool isClear;
	__declspec(property(get = _getSPResult, put = _setSPResult))SpecialResult specialResult;
	__declspec(property(get = _getRecord, put = _setRecord))float clearRate;

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
	const String& getNotesArtistName()const { return  m_notesArtistName; }
	const String& getFileName()const { return m_fileName; }

	const  bool _getIsClear()const { return m_isClear; }
	void _setIsClear(bool newState) { m_isClear = newState; }
	const SpecialResult _getSPResult()const { return m_specialResult; }
	void _setSPResult(SpecialResult sp) { m_specialResult = sp; }
	const float _getRecord()const { return m_clearRate; }
	void _setRecord(float clearRate) { m_clearRate = clearRate; }

	BPMType getCurrentBPM()const
	{
		return m_tempoInfos[m_currentBarIndex].m_bar.getBPM();
	}

	const Color& getColor()const
	{
		return m_color;
	}
	const MusicData* const getMusic()const
	{
		return m_pMusic;
	}
	void load(CSVReader& csv);
};
