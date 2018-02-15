#pragma once
#include"Singleton.h"
#include"Game.h"

#include<queue>
class AutoPlayManager :public Singleton<AutoPlayManager>
{
private:
	struct FrameCount
	{
		FrameCount(int _c=15):
			count(_c)
		{}
		int count=0;
	};

	std::array<FrameCount,3> m_isPressed;

public:
	AutoPlayManager():
		m_autoPlay(false) {}

	bool isRedPressed()const
	{
		return m_isPressed[0].count;
	}
	bool isBluePressed()const
	{
		return m_isPressed[1].count;
	}
	bool isYellowPressed()const
	{
		return m_isPressed[2].count;
	}
	bool m_autoPlay;
	void update() 
	{
		for (auto&& elm : m_isPressed)
		{
			if (elm.count>0)
			{
				--elm.count;
			}
		}
	}
	void input(NoteType type)
	{
		const int fc = type >= 11 && type <= 17 ? 1 : 7;

		switch (type)
		{
		case 1:
		case 11:m_isPressed[0]=FrameCount(fc); break;
		case 2:
		case 12:m_isPressed[1]=FrameCount(fc); break;
		case 3:
		case 13:m_isPressed[2]=FrameCount(fc); break;
		case 4:
		case 14:
			m_isPressed[1]=FrameCount(fc);
			m_isPressed[2]=FrameCount(fc);
			break;
		case 5:
		case 15:
			m_isPressed[0] = FrameCount(fc);
			m_isPressed[2] = FrameCount(fc);
			break;
		case 6:
		case 16:
			m_isPressed[0] = FrameCount(fc);
			m_isPressed[1] = FrameCount(fc);
			break;
		case 7:
		case 17:
			m_isPressed[0] = FrameCount(fc);
			m_isPressed[1] = FrameCount(fc);
			m_isPressed[2] = FrameCount(fc);
		}
	}
	friend class Singleton<AutoPlayManager>;
};
