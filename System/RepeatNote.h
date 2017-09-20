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

	void init()override
	{
		m_isStart = false;
		Note::init();
	}

	bool update(double& nowCount, double& countPerFrame, Score& score, Sound& sound)override
	{
		if (!m_isActive)
			return true;

		const double count= m_count - nowCount;

		//判定範囲まで到達してなければタップ処理を行わない
		if (count > JudgeRange(countPerFrame, Judge::Good))
			return true;

		bool judge = PlayKey::Red().clicked || PlayKey::Blue().clicked || PlayKey::Yellow().clicked;

		if ((judge||count <= 0 )&& m_isStart == false)
		{
			m_isStart = true;
		}



		return true;
	}
	void diffDraw(double count, float scrollRate)const override
	{
		if(this->isFirstTap())
		count = 0;
		const auto pos = getPos(count, scrollRate);

		if (!CanDraw(pos))
			return;

		{
			auto& texture = TextureAsset(L"comet_rainbow_head");
			texture.drawAt(getPos((3 + (-count*scrollRate) / 10000.0)*Pi / 6, count, scrollRate));
			texture.rotate(-4.0*Pi / 3.0).drawAt(getPos((7 + (-count*scrollRate) / 10000.0)* Pi / 6, count, scrollRate));
			texture.rotate(-2.0*Pi / 3.0).drawAt(getPos((11 + (-count*scrollRate) / 10000.0)* Pi / 6, count, scrollRate));
		}
	}

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

	RepeatEnd(double firstCount, double speed, std::shared_ptr<Note>& parent,double interval=8.0):
		LongNote(10,firstCount,speed,parent),
		m_interval(interval)
	{
		//どれかおしたら
		m_judge = []()
		{
			return PlayKey::Red().clicked || PlayKey::Blue().clicked || PlayKey::Yellow().clicked;
		};
	}
	void init()override
	{
		m_isTap = false;
		m_isStart = false;
		LongNote::init();
	}
	bool update(double& nowCount, double& countPerFrame, Score& score, Sound& sound)override;
	void diffDraw(double count, float scrollRate)const override;
};