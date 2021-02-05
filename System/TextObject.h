#pragma once

#include"Object.h"
#include<Siv3D/String.hpp>

class TextObject :public Object
{
private:
	const s3d::String m_msg;	//描画するメッセージ
	const s3d::uint64 m_drawTimeSample;
	s3d::int64 m_beginTimeSample;
	bool m_isDraw;

public:
	TextObject(double firstCount, const s3d::String& msg,const double timeSec) :
		Object(firstCount),
		m_msg(msg),
		m_drawTimeSample(timeSec*44100),
		m_beginTimeSample(0),
		m_isDraw(false)
	{};
	virtual ~TextObject() = default;
	virtual bool update(const double& nowCount, const double& countPerFrame)override;
	virtual void diffDraw(double count, float scrollRate)const override;
	void init()override;
};
