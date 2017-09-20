#pragma once
#include"Singleton.h"
#include"Game.h"

class AutoPlayManager :public Singleton<AutoPlayManager>
{
private:

public:
	AutoPlayManager(): m_isRedPressed(false), m_isBluePressed(false), m_isYellowPressed(false), m_autoPlay(false) {}

	bool m_autoPlay;

	bool m_isRedPressed,m_isBluePressed,m_isYellowPressed;
	void update() 
	{
		m_isRedPressed = m_isBluePressed = m_isYellowPressed = false;
	}
	void input(NoteType type)
	{
		switch (type)
		{
		case 1:
		case 11:m_isRedPressed = true; break;
		case 2:
		case 12:m_isBluePressed = true; break;
		case 3:
		case 13:m_isYellowPressed = true; break;
		case 4:
		case 14:
			m_isBluePressed = true;
			m_isYellowPressed = true;
			break;
		case 5:
		case 15:
			m_isRedPressed = true;
			m_isYellowPressed = true;
			break;
		case 6:
		case 16:
			m_isRedPressed = true;
			m_isBluePressed = true;
			break;
		case 7:
			m_isRedPressed = true;
			m_isBluePressed = true;
			m_isYellowPressed = true;
		}
	}
	friend class Singleton<AutoPlayManager>;
};
