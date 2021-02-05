#pragma once
#include"LongNote.h"
#include"PlayKey.h"
#include"eJudge.h"

//連打ノーツ
class RepeatNote:public Note 
{
private:

	//連打開始
	bool m_isStart=false;

public:
	RepeatNote(double firstCount, double speed):
		Note(10,firstCount,speed)
	{}

	void init()override;

	bool update(const double& nowCount, const double& countPerFrame)override;
	void diffDraw(double count, float scrollRate)const override;

	bool isFirstTap()const override
	{
		return m_isStart;
	}
};

//連打の終点
class RepeatEnd :public LongNote
{
private:
	double m_lastCount = 0;
	bool m_isStart = false;
	bool m_isTap = false;
	double m_interval;//連打間隔
public:
	static double notesTapCount;

	RepeatEnd(double firstCount, double speed, std::shared_ptr<Note>& parent, double interval = 8.0);
	void init()override;
	bool update(const double& nowCount,const double& countPerFrame)override;
	void diffDraw(double count, float scrollRate)const override;
};