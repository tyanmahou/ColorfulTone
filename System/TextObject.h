#pragma once

#pragma once
#include"Object.h"

class TextObject :public Object
{
private:
	const String m_msg;	//描画するメッセージ
	const uint64 m_drawTimeSample;
	int64 m_beginTimeSample;
	bool m_isDraw;

public:
	TextObject(double firstCount, const String& msg,const double timeSec) :
		Object(firstCount),
		m_msg(msg),
		m_drawTimeSample(timeSec*44100),
		m_beginTimeSample(0),
		m_isDraw(false)
	{};
	virtual ~TextObject() = default;
	virtual bool update(double& nowCount, double& countPerFrame, Score& score, Sound& sound)override;
	virtual void diffDraw(double count, float scrollRate)const override;
	void init()override;
};
