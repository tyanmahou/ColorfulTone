#include"Note.h"
#include"Useful.hpp"
#include"eJudge.h"
#include"AutoPlayManager.h"
#include"InputManager.hpp"

#include"PlayMusicGame.h"

#include"RepeatNote.h"
#include"PlayStyle.h"
namespace {
	//--------------------------------------------------------------------------------
	//関数：GetColor
	//--------------------------------------------------------------------------------
	//概要：ノーツの色を取得
	//--------------------------------------------------------------------------------

	Color GetColor(const NoteType type)
	{
		switch (type)
		{
		case 1:
		case 11:return Palette::Red;
		case 2:
		case 12:return Palette::Blue;
		case 3:
		case 13:return Palette::Yellow;
		case 4:
		case 14:return Palette::Green;
		case 5:
		case 15:return Palette::Orange;
		case 6:
		case 16:return Palette::Purple;
		}
		return Palette::Black;
	}
}

//--------------------------------------------------------------------------------
//関数：コンストラクタ
//--------------------------------------------------------------------------------


Note::Note(const NoteType type, double firstCount, double speed) :
	Object(firstCount),
	m_type(type),
	m_scrollSpeed(speed)
{
	m_isClicked[0] = false;
	m_isClicked[1] = false;
	m_isClicked[2] = false;

	m_color = GetColor(type);

	//アクション
	switch (type)
	{
	case 1:
	case 11:
		m_judge = [&]()
		{
			if (InputManager::IsRedClicked() && !m_isClicked[ColorIndex::Red])
			{
				InputManager::PopRed();
				m_isClicked[ColorIndex::Red] = true;
			}
			return m_isClicked[ColorIndex::Red];
		};
		break;
	case 2:
	case 12:
		m_judge = [&]()
		{
			if (InputManager::IsBlueClicked() && !m_isClicked[ColorIndex::Blue])
			{
				InputManager::PopBlue();
				m_isClicked[ColorIndex::Blue] = true;
			}
			return m_isClicked[ColorIndex::Blue];
		};
		break;
	case 3:
	case 13:
		m_judge = [&]()
		{
			if (InputManager::IsYellowClicked() && !m_isClicked[ColorIndex::Yellow])
			{
				InputManager::PopYellow();
				m_isClicked[ColorIndex::Yellow] = true;
			}
			return m_isClicked[ColorIndex::Yellow];
		};
		break;
	case 4:
	case 14:
		m_judge = [&]()
		{
			if (InputManager::IsBlueClicked() && !m_isClicked[ColorIndex::Blue])
			{
				InputManager::PopBlue();
				m_isClicked[ColorIndex::Blue] = true;
			}
			if (InputManager::IsYellowClicked() && !m_isClicked[ColorIndex::Yellow])
			{
				InputManager::PopYellow();
				m_isClicked[ColorIndex::Yellow] = true;
			}
			return m_isClicked[ColorIndex::Blue] && m_isClicked[ColorIndex::Yellow];
		};
		break;
	case 5:
	case 15:
		m_judge = [&]()
		{
			if (InputManager::IsRedClicked() && !m_isClicked[ColorIndex::Red])
			{
				InputManager::PopRed();
				m_isClicked[ColorIndex::Red] = true;
			}
			if (InputManager::IsYellowClicked() && !m_isClicked[ColorIndex::Yellow])
			{
				InputManager::PopYellow();
				m_isClicked[ColorIndex::Yellow] = true;
			}
			return m_isClicked[ColorIndex::Red] && m_isClicked[ColorIndex::Yellow];
		};
		break;
	case 6:
	case 16:
		m_judge = [&]()
		{
			if (InputManager::IsRedClicked() && !m_isClicked[ColorIndex::Red])
			{
				InputManager::PopRed();
				m_isClicked[ColorIndex::Red] = true;
			}
			if (InputManager::IsBlueClicked() && !m_isClicked[ColorIndex::Blue])
			{
				InputManager::PopBlue();
				m_isClicked[ColorIndex::Blue] = true;
			}
			return m_isClicked[ColorIndex::Red] && m_isClicked[ColorIndex::Blue];
		};
		break;
	case 7:
	case 17:
		m_judge = [&]()
		{
			if (InputManager::IsRedClicked() && !m_isClicked[ColorIndex::Red])
			{
				InputManager::PopRed();
				m_isClicked[ColorIndex::Red] = true;
			}
			if (InputManager::IsBlueClicked() && !m_isClicked[ColorIndex::Blue])
			{
				InputManager::PopBlue();
				m_isClicked[ColorIndex::Blue] = true;
			}
			if (InputManager::IsYellowClicked() && !m_isClicked[ColorIndex::Yellow])
			{
				InputManager::PopYellow();
				m_isClicked[ColorIndex::Yellow] = true;
			}
			return m_isClicked[ColorIndex::Red] && m_isClicked[ColorIndex::Blue] && m_isClicked[ColorIndex::Yellow];
		};
		break;
	case 18:
		m_judge = [&]()
		{
			if (InputManager::IsRedClicked())
			{
				InputManager::PopRed();
				m_isClicked[ColorIndex::Red] = true;
			}
			if (InputManager::IsBlueClicked())
			{
				InputManager::PopBlue();
				m_isClicked[ColorIndex::Blue] = true;
			}
			if (InputManager::IsYellowClicked())
			{
				InputManager::PopYellow();
				m_isClicked[ColorIndex::Yellow] = true;
			}
			return m_isClicked[ColorIndex::Red] || m_isClicked[ColorIndex::Blue] || m_isClicked[ColorIndex::Yellow];
		};
		break;
	case 9:
		m_judge = [&]()
		{
			if (InputManager::IsRedClicked())
			{
				m_isClicked[ColorIndex::Red] = true;
			}
			if (InputManager::IsBlueClicked())
			{
				m_isClicked[ColorIndex::Blue] = true;
			}
			if (InputManager::IsYellowClicked())
			{
				m_isClicked[ColorIndex::Yellow] = true;
			}
			return m_isClicked[ColorIndex::Red] || m_isClicked[ColorIndex::Blue] || m_isClicked[ColorIndex::Yellow];
		};
		break;
	default:
		m_judge = []() {return false; };
	}
};

//--------------------------------------------------------------------------------
//関数：init
//--------------------------------------------------------------------------------
//概要：初期化
//--------------------------------------------------------------------------------

void Note::init()
{
	Object::init();
	m_isClicked[ColorIndex::Red] = false;
	m_isClicked[ColorIndex::Blue] = false;
	m_isClicked[ColorIndex::Yellow] = false;
}

//--------------------------------------------------------------------------------
//関数：tapUpdate
//--------------------------------------------------------------------------------
//概要：タップ成功時の処理
//--------------------------------------------------------------------------------
void Note::tapUpdate(Score::Judge judge)
{
	// 白の場合ミス
	if (m_type == 9)
	{
		judge = Score::Miss;
	}
	//虹色単ノーツは必ずパフェ
	if (m_type == 18)
	{
		judge = Score::Perfect;
	}

	if (m_type <= 7 || m_type == 9 || m_type == 18)
	{
		m_isActive = false;
	}

	NoteType type = m_type;
	if (m_type == 18)
	{
		//　虹色単色の場合押した場所にエフェクトを出す
		type = m_isClicked[ColorIndex::Red] ? 1
			: m_isClicked[ColorIndex::Blue] ? 2
			: 3;
	}
	PlayMusicGame::ScoreUpdate(judge, type);
}

//--------------------------------------------------------------------------------
//関数：tapMiss
//--------------------------------------------------------------------------------
//概要：タップミス時の処理
//--------------------------------------------------------------------------------

void Note::tapMiss()
{
	Score::Judge judge = Score::Miss;
	// 白の場合はパフェ
	if (m_type == 9)
	{
		judge = Score::Perfect;
	}

	PlayMusicGame::ScoreUpdate(judge, m_type, false);
	/*
	ロングノーツの場合は、始点が押せなかった時点で終点分も同時にミスとする。
	*/
	if (11 <= m_type && m_type <= 17)
	{
		PlayMusicGame::ScoreUpdate(judge, m_type, false);
	}
	m_isActive = false;
}
//--------------------------------------------------------------------------------
//関数：update
//--------------------------------------------------------------------------------

bool Note::update(double& nowCount, double& countPerFrame)
{
	if (!m_isActive)
		return true;

	//ロング用 始点が押されてたらそのままつぎののーつの判定に
	if (isFirstTap())
	{
		if (AutoPlayManager::IsAutoPlay())
			AutoPlayManager::Input(m_type);
		return true;
	}
	auto count = m_count - nowCount;

	//ミス
	if (count < -JudgeRange(countPerFrame, Judge::Good) || (m_type == 9 && count <= 0))
	{
		this->tapMiss();
		return true;
	}

	//判定範囲まで到達してなければタップ処理を行わない
	if (count > JudgeRange(countPerFrame, Judge::Good))
		return true;

	//オートプレイ---------------------------------
	if (AutoPlayManager::IsAutoPlay())
	{
		if (count <= countPerFrame && m_type != 9)
		{
			int type = m_type;
			if (type == 18)
			{
				static int tap = 0;
				++tap %= 3;
				type = tap + 1;
				m_isClicked[tap] = true;
			}
			else {
				m_isClicked[ColorIndex::Red] = true;
				m_isClicked[ColorIndex::Blue] = true;
				m_isClicked[ColorIndex::Yellow] = true;
			}
			this->tapUpdate(Score::Perfect);

			AutoPlayManager::Input(type);
			return false;
		}
		return true;
	}


	//---------------------------------------------

	//遅れてしまったか
	if (m_judge())
	{
		auto aCount = Abs(count);
		if (aCount <= JudgeRange(countPerFrame, Judge::Perfect))
		{
			this->tapUpdate(Score::Perfect);
		}
		else if (aCount <= JudgeRange(countPerFrame, Judge::Great))
		{
			this->tapUpdate(Score::Great);
		}
		else if (aCount <= JudgeRange(countPerFrame, Judge::Good))
		{
			this->tapUpdate(Score::Good);
		}

		RepeatEnd::notesTapCount = nowCount;

		return InputManager::IsAnyClicked();
	}
	return true;
}

//----------------------------------------------------------------------------
//関数：draw
//----------------------------------------------------------------------------

void Note::diffDraw(double count, float scrollRate) const
{
	PlayStyle::Instance()->draw(*this, count, scrollRate);
}


//--------------------------------------------------------------------------------
//関数：isFirstTap
//--------------------------------------------------------------------------------
//概要：とりあえず一回はおされたかどうか
//--------------------------------------------------------------------------------
bool Note::isFirstTap() const
{
	switch (m_type)
	{
	case 1:
	case 11:
		return m_isClicked[ColorIndex::Red];
	case 2:
	case 12:
		return m_isClicked[ColorIndex::Blue];
	case 3:
	case 13:
		return m_isClicked[ColorIndex::Yellow];
	case 4:
	case 14:
		return m_isClicked[ColorIndex::Blue] && m_isClicked[ColorIndex::Yellow];
	case 5:
	case 15:
		return m_isClicked[ColorIndex::Red] && m_isClicked[ColorIndex::Yellow];
	case 6:
	case 16:
		return m_isClicked[ColorIndex::Red] && m_isClicked[ColorIndex::Blue];
	case 7:
	case 17:
		return m_isClicked[ColorIndex::Red] && m_isClicked[ColorIndex::Blue] && m_isClicked[ColorIndex::Yellow];
	case 18:
		return m_isClicked[ColorIndex::Red] || m_isClicked[ColorIndex::Blue] || m_isClicked[ColorIndex::Yellow];
	}
	return false;
}