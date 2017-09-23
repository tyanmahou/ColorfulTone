#include"Note.h"
#include"PlayKey.h"
#include"eJudge.h"
#include"AutoPlayManager.h"
#include"TapEffect.h"
#include"JudgeEffect.h"
#include"PlayMusicGame.h"

#include"RepeatNote.h"
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

	//--------------------------------------------------------------------------------
	//関数：GetAngle
	//--------------------------------------------------------------------------------
	//概要：ノーツが来る角度を取得
	//--------------------------------------------------------------------------------

	double GetAngle(NoteType type) {
		static const std::unordered_map<NoteType, double> angleMap
		{
			{ 1,7 * Pi / 6 },
			{ 2, Pi / 2 },
			{ 3,11 * Pi / 6 },
			{ 4,Pi / 6 },
			{ 5,3 * Pi / 2 },
			{ 6,5 * Pi / 6 },
			{ 7,0 },

		};
		if (angleMap.count(type))
			return angleMap.at(type);

		return 0.0;
	}

	//--------------------------------------------------------------------------------
	//関数：GetTextureAngle
	//--------------------------------------------------------------------------------
	//概要：ノーツのテクスチャの角度を取得
	//--------------------------------------------------------------------------------

	double GetTextureAngle(NoteType type)
	{
		static const std::unordered_map<NoteType, double> textureAngleMap
		{
			{ 1,-4.0*Pi / 3.0 },
			{ 2,0.0 },
			{ 3,-2.0*Pi / 3.0 },
			{ 4,-Pi / 3.0 },
			{ 5,-Pi },
			{ 6,-5.0*Pi / 3.0 },
			{ 7,0.0 },

		};
		if (textureAngleMap.count(type))
			return textureAngleMap.at(type);

		return 0.0;
	}

	//--------------------------------------------------------------------------------
	//関数：GetTxetureName
	//--------------------------------------------------------------------------------
	//概要：ノーツのテクスチャの名前を取得
	//--------------------------------------------------------------------------------

	String GetTxetureName(NoteType type)
	{
		static const std::unordered_map<NoteType, String> textureNameMap
		{
			{ 1,L"note_red"},
			{ 2,L"note_blue"},
			{ 3,L"note_yellow" },
			{ 4,L"note_green" },
			{ 5,L"note_orange" },
			{ 6,L"note_purple" },
			{ 7,L"note_black" },
			{ 9,L"note_white" },
			{ 11,L"comet_red" },
			{ 12,L"comet_blue" },
			{ 13,L"comet_yellow" },
			{ 14,L"comet_green" },
			{ 15,L"comet_orange" },
			{ 16,L"comet_purple" },
			{ 17,L"comet_black" },
		};
		if (textureNameMap.count(type))
			return textureNameMap.at(type);

		return L"note_black";
	}

}

//--------------------------------------------------------------------------------
//関数：コンストラクタ
//--------------------------------------------------------------------------------


Note::Note(const NoteType type, double firstCount, double speed) :
	Object(firstCount),
	m_type(type),
	m_scrollSpeed(speed),
	m_textureName(GetTxetureName(type)),
	m_angle(GetAngle(type % 10)),
	m_textureAngle(GetTextureAngle(type % 10))
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
		PlayMusicGame::GetEffect().add<JudgeEffect>(L"MISS", getPos(3 * Pi / 2, 2400, 1.0f, 1.0));
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
		PlayMusicGame::GetEffect().add<TapEffect>(1 * Pi / 6, 7);
		PlayMusicGame::GetEffect().add<TapEffect>(9 * Pi / 6, 7);
		PlayMusicGame::GetEffect().add<TapEffect>(5 * Pi / 6, 7);
		PlayMusicGame::GetEffect().add<JudgeEffect>(scoreMap.at(judge), getPos(3 * Pi / 2, 2400, 1.0f, 1.0));
	}
	else
	{
		PlayMusicGame::GetEffect().add<TapEffect>(m_angle, m_type % 10);
		PlayMusicGame::GetEffect().add<JudgeEffect>(scoreMap.at(judge), getPos(m_angle, 2400, 1.0f, 1.0));
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
		PlayMusicGame::GetEffect().add<TapEffect>(0, 9);
		PlayMusicGame::GetEffect().add<TapEffect>(Pi, 9);
		PlayMusicGame::GetEffect().add<JudgeEffect>(L"PERFECT", getPos(3 * Pi / 2, 2400, 1.0f, 1.0));
		score.m_judgeCount[Score::Perfect]++;
		score.m_currentCombo++;
		m_isActive = false;
		return;
	}
	score.m_currentCombo = 0;
	if (m_type % 10 == 7)
		PlayMusicGame::GetEffect().add<JudgeEffect>(L"MISS", getPos(3 * Pi / 2, 2400, 1.0f, 1.0));
	else
		PlayMusicGame::GetEffect().add<JudgeEffect>(L"MISS", getPos(m_angle, 2400, 1.0f, 1.0));

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

	if (m_type >= 11 && (count <= 0 && isFirstTap() || isFirstTap()))
		count = 0;

	const auto pos = getPos(count, scrollRate);

	if (!CanDraw(pos))
		return;

	if (m_type % 10 == 7)
	{

		TextureAsset(m_textureName).rotate(-Pi / 3.0).drawAt(getPos((1 + (count*scrollRate) / 10000.0)*Pi / 6, count, scrollRate));
		TextureAsset(m_textureName).rotate(-5.0*Pi / 3.0).drawAt(getPos((5 + (count*scrollRate) / 10000.0)* Pi / 6, count, scrollRate));
		TextureAsset(m_textureName).rotate(-Pi).drawAt(getPos((9 + (count*scrollRate) / 10000.0)* Pi / 6, count, scrollRate));
		return;
	}
	else if (m_type == 9)
	{
		TextureAsset(L"note_white").drawAt(getPos(Pi, count, scrollRate));
	}

	TextureAsset(m_textureName).rotate(m_textureAngle).drawAt(pos);
}

//--------------------------------------------------------------------------------
//関数：getPos
//--------------------------------------------------------------------------------
//概要：描画座標取得
//--------------------------------------------------------------------------------
const Vec2 Note::getPos(double angle, double count, float scrollRate, double scrollSpeed) const
{
	Vec2 pos;
	pos.x = 400 + 40 * cos(angle) + (count / Object::RESOLUTION * scrollRate*scrollSpeed)*cos(angle);
	pos.y = 300 + 40 * sin(angle) + (count / Object::RESOLUTION * scrollRate*scrollSpeed)*sin(angle);
	return pos;
}

const Vec2 Note::getPos(double angle, double count, float scrollRate)const
{
	return getPos(angle, count, scrollRate, m_scrollSpeed);
}

const Vec2 Note::getPos(double count, float scrollRate) const
{
	return getPos(m_angle, count, scrollRate);
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