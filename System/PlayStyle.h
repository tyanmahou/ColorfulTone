#pragma once

#include"Singleton.h"
#include"NormalStyle.h"
#include"Portrait.h"

enum class PlayStyleType
{
	Normal,
	Portrait,
	Default = Normal
};

class PlayStyle :public Singleton<PlayStyle>
{

	std::shared_ptr<IPlayStyle> m_style;

	PlayStyle() :
		m_style(std::make_shared<NormalStyle>())
	{}

public:
	void setStyle(PlayStyleType type)
	{

		static const std::unordered_map<PlayStyleType, std::shared_ptr<IPlayStyle>> factory
		{
			{PlayStyleType::Normal,std::make_shared<NormalStyle>()},
			{ PlayStyleType::Portrait,std::make_shared<Portrait>() },
		};

		m_style = factory.at(type);
	}
	template<class T>
	void draw(const T& note, double count, float scrollRate)const
	{
		m_style->draw(note, count, scrollRate);
	}

	void drawJudgeLine() const
	{
		m_style->drawJudgeLine();
	}

	void drawFrame(bool red, bool blue, bool yellow,std::function<void()>drawCallback)const
	{
		m_style->drawFrame(red, blue, yellow, drawCallback);
	}

	void drawComboAndRate(int combo,float rate)const
	{
		m_style->drawComboAndRate(combo, rate);
	}

	void drawTapEffect(int type)const
	{
		m_style->drawTapEffect(type);
	}
	void drawJudgeEffect(const String& str,int type)const
	{
		m_style->drawJudgeEffect(str,type);
	}
	friend Singleton<PlayStyle>;
};

