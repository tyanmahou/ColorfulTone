#include"RepeatNote.h"
#include"Useful.hpp"
#include"AutoPlayManager.h"
#include"PlayMusicGame.h"
#include"PlayStyle.h"
namespace
{
	void EffectUpdate(bool red, bool blue, bool yellow)
	{
		if (red)
			PlayStyle::Instance()->drawTapEffect(1);
		if (blue)
			PlayStyle::Instance()->drawTapEffect(2);
		if (yellow)
			PlayStyle::Instance()->drawTapEffect(3);
		SoundManager::SE::Play(L"PERFECT");
	}
}

double RepeatEnd::notesTapCount = 0;

RepeatEnd::RepeatEnd(double firstCount, double speed, std::shared_ptr<Note>& parent, double interval):
	LongNote(10, firstCount, speed, parent),
	m_interval(interval)
{
	//どれかおしたら
	m_judge = []()
	{
		return PlayKey::Red().clicked || PlayKey::Blue().clicked || PlayKey::Yellow().clicked;
	};
}

void RepeatEnd::init()
{
	m_isTap = false;
	m_isStart = false;
	LongNote::init();
}

bool RepeatEnd::update(double & nowCount, double & countPerFrame, Score & score, Sound & sound)
{
	if (!m_isActive || !m_parent->isFirstTap())
		return true;

	auto count = m_count - nowCount;

	//初期化
	if (!m_isStart)
	{
		m_isStart = true;
		m_lastCount = nowCount;
	}
	//オートプレイ----------------------
	if (AutoPlayManager::IsAutoPlay())
	{
		if (m_lastCount == nowCount||nowCount > m_lastCount + NotesData::RESOLUTION / (m_interval * 2))
		{
			static int tap = 0;
			++tap %= 3;
			AutoPlayManager::Input(tap+1);
			EffectUpdate(tap == 0, tap == 1, tap == 2);
			m_lastCount = nowCount;
			m_isTap = true;
		}
	}
	else
		//----------------------------------
		if (m_judge())
		{
			EffectUpdate(PlayKey::Red().clicked, PlayKey::Blue().clicked, PlayKey::Yellow().clicked);
			m_lastCount = nowCount;
			m_isTap = true;
		}

	if (notesTapCount > m_lastCount)
	{
		m_lastCount = notesTapCount;
		m_isTap = true;
	}


	if (count <= 0)//ロングの終点
	{
		if (m_isTap)
			perfect(score);
		else
			miss(score);

		return false;
	}
	//8分間隔以内で連打しないとミス
	if (nowCount > m_lastCount + NotesData::RESOLUTION / m_interval)
	{
		auto aCount = Abs(count);
		//パーフェクト範囲内での話はセーフ
		if (aCount <= JudgeRange(countPerFrame, Judge::Perfect) && m_isTap)
			perfect(score);
		else
			miss(score);
	}
	return false;
}

void RepeatEnd::diffDraw(double count, float scrollRate) const
{
	PlayStyle::Instance()->draw(*this, count, scrollRate);
}

void RepeatNote::init()
{
	m_isStart = false;
	Note::init();
}
bool RepeatNote::update(double & nowCount, double & countPerFrame, Score & score, Sound & sound)
{
	if (!m_isActive)
		return true;

	const double count = m_count - nowCount;

	//判定範囲まで到達してなければタップ処理を行わない
	if (count > JudgeRange(countPerFrame, Judge::Good))
		return true;

	bool judge = PlayKey::Red().clicked || PlayKey::Blue().clicked || PlayKey::Yellow().clicked;

	if ((judge || count <= 0) && m_isStart == false)
	{
		m_isStart = true;
	}
	return true;
}
void RepeatNote::diffDraw(double count, float scrollRate) const
{
	PlayStyle::Instance()->draw(*this, count, scrollRate);

}
