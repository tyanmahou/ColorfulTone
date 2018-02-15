#include"Note.h"
#include"PlayKey.h"
#include"eJudge.h"
#include"AutoPlayManager.h"

#include"PlayMusicGame.h"

#include"RepeatNote.h"
#include"PlayStyle.h"
namespace {
	//--------------------------------------------------------------------------------
	//関数：IsNeedBuutonClick
	//--------------------------------------------------------------------------------
	//概要：必要なボタンがクリックされたか
	//--------------------------------------------------------------------------------

	bool IsNeedBuutonClick(const NoteType type)
	{
		const auto tmp = type % 10;
		switch (tmp)
		{
		case 1:return PlayKey::Red().clicked;
		case 2:return PlayKey::Blue().clicked;
		case 3:return PlayKey::Yellow().clicked;
		case 4:return PlayKey::Blue().clicked || PlayKey::Yellow().clicked;
		case 5:return PlayKey::Red().clicked || PlayKey::Yellow().clicked;
		case 6:return PlayKey::Blue().clicked || PlayKey::Red().clicked;
		case 7:return PlayKey::Blue().clicked || PlayKey::Yellow().clicked || PlayKey::Red().clicked;
		}
		return true;
	}

	//--------------------------------------------------------------------------------
	//関数：GetColor
	//--------------------------------------------------------------------------------
	//概要：ノーツの色を取得
	//--------------------------------------------------------------------------------

	Color GetColor(const NoteType type)
	{
		switch (type % 10)
		{
		case 1:return Palette::Red;
		case 2:return Palette::Blue;
		case 3:return Palette::Yellow;
		case 4:return Palette::Green;
		case 5:return Palette::Orange;
		case 6:return Palette::Purple;

		}
		return Palette::Black;
	}
}

//--------------------------------------------------------------------------------
//関数：コンストラクタ
//--------------------------------------------------------------------------------


Note::Note(const NoteType type, double firstCount, double speed):
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
			if (PlayKey::Red().clicked && !m_isClicked[ColorIndex::Red])
			{
				m_isAnyClicked = true;
				m_isClicked[ColorIndex::Red] = true;
			}
			return m_isClicked[ColorIndex::Red];
		};
		break;
	case 2:
	case 12:
		m_judge = [&]()
		{
			if (PlayKey::Blue().clicked && !m_isClicked[ColorIndex::Blue])
			{
				m_isAnyClicked = true;
				m_isClicked[ColorIndex::Blue] = true;
			}
			return m_isClicked[ColorIndex::Blue];
		};
		break;
	case 3:
	case 13:
		m_judge = [&]()
		{
			if (PlayKey::Yellow().clicked && !m_isClicked[ColorIndex::Yellow])
			{
				m_isAnyClicked = true;
				m_isClicked[ColorIndex::Yellow] = true;
			}
			return m_isClicked[ColorIndex::Yellow];
		};
		break;
	case 4:
	case 14:
		m_judge = [&]()
		{
			if (PlayKey::Blue().clicked && !m_isClicked[ColorIndex::Blue])
			{
				m_isAnyClicked = true;
				m_isClicked[ColorIndex::Blue] = true;
			}
			if (PlayKey::Yellow().clicked && !m_isClicked[ColorIndex::Yellow])
			{
				m_isAnyClicked = true;
				m_isClicked[ColorIndex::Yellow] = true;
			}
			return m_isClicked[ColorIndex::Blue] && m_isClicked[ColorIndex::Yellow];

		};
		break;
	case 5:
	case 15:
		m_judge = [&]()
		{
			if (PlayKey::Red().clicked && !m_isClicked[ColorIndex::Red])
			{
				m_isAnyClicked = true;
				m_isClicked[ColorIndex::Red] = true;
			}
			if (PlayKey::Yellow().clicked && !m_isClicked[ColorIndex::Yellow])
			{
				m_isAnyClicked = true;
				m_isClicked[ColorIndex::Yellow] = true;
			}
			return m_isClicked[ColorIndex::Red] && m_isClicked[ColorIndex::Yellow];
		};
		break;
	case 6:
	case 16:
		m_judge = [&]()
		{
			if (PlayKey::Red().clicked && !m_isClicked[ColorIndex::Red])
			{
				m_isAnyClicked = true;
				m_isClicked[ColorIndex::Red] = true;
			}
			if (PlayKey::Blue().clicked && !m_isClicked[ColorIndex::Blue])
			{
				m_isAnyClicked = true;
				m_isClicked[ColorIndex::Blue] = true;
			}
			return m_isClicked[ColorIndex::Red] && m_isClicked[ColorIndex::Blue];


		};
		break;
	case 17:
	case 7:m_judge = [&]()
	{
		if (PlayKey::Red().clicked && !m_isClicked[ColorIndex::Red])
		{
			m_isAnyClicked = true;
			m_isClicked[ColorIndex::Red] = true;
		}
		if (PlayKey::Blue().clicked && !m_isClicked[ColorIndex::Blue])
		{
			m_isAnyClicked = true;
			m_isClicked[ColorIndex::Blue] = true;
		}
		if (PlayKey::Yellow().clicked && !m_isClicked[ColorIndex::Yellow])
		{
			m_isAnyClicked = true;
			m_isClicked[ColorIndex::Yellow] = true;
		}
		return m_isClicked[ColorIndex::Red] && m_isClicked[ColorIndex::Blue] && m_isClicked[ColorIndex::Yellow];
	};
		   break;
	case 9:
		m_judge = [&]() {
			if (PlayKey::Red().clicked)
				m_isClicked[ColorIndex::Red] = true;
			if (PlayKey::Blue().clicked)
				m_isClicked[ColorIndex::Blue] = true;
			if (PlayKey::Yellow().clicked)
				m_isClicked[ColorIndex::Yellow] = true;
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
	m_isAnyClicked = false;

}

//--------------------------------------------------------------------------------
//関数：tapUpdate
//--------------------------------------------------------------------------------
//概要：タップ成功時の処理
//--------------------------------------------------------------------------------
void Note::tapUpdate(Score::Judge judge, Score& score)
{
	static const std::unordered_map<Score::Judge, String> scoreMap
	{
		{ Score::Good,L"GOOD" },
		{ Score::Great,L"GREAT" },
		{ Score::Perfect,L"PERFECT" },
	};

	if (m_type == 9)
	{
		score.m_currentCombo = 0;
		PlayStyle::Instance()->drawJudgeEffect(L"MISS",9);
		score.m_judgeCount[Score::Miss]++;
		m_isActive = false;
		return;
	}
	SoundManager::SE::Play(scoreMap.at(judge));


	if (m_type <= 7)
		m_isActive = false;

	score.m_currentCombo++;
	score.m_judgeCount[judge]++;
	if (m_type % 10 == 7)
	{
		PlayStyle::Instance()->drawTapEffect(7);
		PlayStyle::Instance()->drawJudgeEffect(scoreMap.at(judge),7);
	}
	else
	{
		PlayStyle::Instance()->drawTapEffect(m_type%10);
		PlayStyle::Instance()->drawJudgeEffect(scoreMap.at(judge), m_type%10);
	}
}

//--------------------------------------------------------------------------------
//関数：tapMiss
//--------------------------------------------------------------------------------
//概要：タップミス時の処理
//--------------------------------------------------------------------------------

void Note::tapMiss(Score& score)
{
	if (m_type == 9)
	{
		PlayStyle::Instance()->drawTapEffect(9);
		PlayStyle::Instance()->drawTapEffect(9);

		PlayStyle::Instance()->drawJudgeEffect(L"PERFECT", 9);
		score.m_judgeCount[Score::Perfect]++;
		score.m_currentCombo++;
		m_isActive = false;
		return;
	}
	score.m_currentCombo = 0;
	if (m_type % 10 == 7)
		PlayStyle::Instance()->drawJudgeEffect(L"MISS", 7);
	else
		PlayStyle::Instance()->drawJudgeEffect(L"MISS",m_type%10);

	score.m_judgeCount[Score::Miss]++;

	/*
	ロングノーツの場合は、始点が押せなかった時点で終点分も同時にミスとする。
	*/
	if (m_type >= 11)
	{
		score.m_judgeCount[Score::Miss]++;
	}
	m_isActive = false;
}
//--------------------------------------------------------------------------------
//関数：update
//--------------------------------------------------------------------------------

bool Note::update(double& nowCount, double& countPerFrame, Score& score, Sound& sound)
{
	if (!m_isActive)
		return true;

	//ロング用 始点が押されてたらそのままつぎののーつの判定に
	if (isFirstTap())
	{
		if (AutoPlayManager::Instance()->m_autoPlay)
			AutoPlayManager::Instance()->input(m_type);
		return true;
	}
	auto count = m_count - nowCount;
	m_isAnyClicked = false;

	//ミス
	if (count < -JudgeRange(countPerFrame, Judge::Good) || (m_type == 9 && count <= 0))
	{
		tapMiss(score);
		return true;
	}

	//判定範囲まで到達してなければタップ処理を行わない
	if (count > JudgeRange(countPerFrame, Judge::Good))
		return true;

	//オートプレイ---------------------------------
	if (AutoPlayManager::Instance()->m_autoPlay)
	{
		if (count <= countPerFrame&&m_type != 9)
		{
			m_isClicked[ColorIndex::Red] = true;
			m_isClicked[ColorIndex::Blue] = true;
			m_isClicked[ColorIndex::Yellow] = true;
			tapUpdate(Score::Perfect, score);
			AutoPlayManager::Instance()->input(m_type);
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
			tapUpdate(Score::Perfect, score);
		}
		else if (aCount <= JudgeRange(countPerFrame, Judge::Great))
		{
			tapUpdate(Score::Great, score);
		}
		else if (aCount <= JudgeRange(countPerFrame, Judge::Good))
		{
			tapUpdate(Score::Good, score);
		}
		
		RepeatEnd::notesTapCount = nowCount;

		return false; //count < -JudgeRange(countPerFrame, Judge::Perfect);
	}
	return !m_isAnyClicked;//IsNeedBuutonClick(m_type);
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

	}
	return false;
}