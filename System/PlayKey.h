#pragma once
#include"KeyConfig.h"
class PlayKey 
{
public:

	static KeyCombination& Red() { return m_Red; }
	static KeyCombination& Blue(){ return m_Blue; }
	static KeyCombination& Yellow() { return m_Yellow; }

	static  KeyCombination& Up() { return m_Up; }
	static  KeyCombination& Down() { return m_Down; }
	static  KeyCombination& Right() { return m_Right; }
	static  KeyCombination& Left() { return m_Left; }

	static  KeyCombination& Start() { return m_Start; }
	static  KeyCombination& BigBack() { return m_BigBack; }
	static  KeyCombination& SmallBack() { return m_SmallBack; }


	//キーの初期化
	static bool Init();

private:
	//キー
	static KeyCombination m_Red, m_Blue, m_Yellow,m_Start,m_SmallBack,m_BigBack,m_Up,m_Down,m_Right,m_Left;

	//キーの保存
	static void Save();
	//キー組合せの更新
	static void Update();
	//キーの初期化
	static void Reset();

	friend class PlayKeyConfig;
	friend class DetailKeyConfig;
	PlayKey();
};
