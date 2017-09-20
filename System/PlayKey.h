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


	//�L�[�̏�����
	static bool Init();

private:
	//�L�[
	static KeyCombination m_Red, m_Blue, m_Yellow,m_Start,m_SmallBack,m_BigBack,m_Up,m_Down,m_Right,m_Left;

	//�L�[�̕ۑ�
	static void Save();
	//�L�[�g�����̍X�V
	static void Update();
	//�L�[�̏�����
	static void Reset();

	friend class PlayKeyConfig;
	friend class DetailKeyConfig;
	PlayKey();
};
