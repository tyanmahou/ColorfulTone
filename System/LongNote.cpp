#include"LongNote.h"
#include"PlayKey.h"
#include"eJudge.h"
#include"AutoPlayManager.h"
#include"PlayMusicGame.h"
#include"PlayStyle.h"
namespace 
{

}
LongNote::LongNote(int type, double firstCount,  double speed, std::shared_ptr<Note>& parent) :
	Object(firstCount),
	m_type(type), 
	m_scrollSpeed(speed), 
	m_parent(parent)
{
	m_color = m_parent->getColor();
	HSV hsv;
	hsv.convertFrom(m_color.r/255.0, m_color.g / 255.0, m_color.b / 255.0);
	hsv.s = Min(0.5,hsv.s);
	hsv.v = 1.0;
	m_color=hsv.toColor();
	//判定処理
	switch (type)
	{
	case 11:
		m_judge = []() {
			return PlayKey::Red().pressed;
		};
		break;
	case 12:
		m_judge = []() {
			return PlayKey::Blue().pressed;
		};
		break;
	case 13:
		m_judge = []() {
			return PlayKey::Yellow().pressed;
		};
		break;
	case 14:
		m_judge = []() {
			return PlayKey::Blue().pressed && PlayKey::Yellow().pressed;
		};
		break;
	case 15:
		m_judge = []() {
			return PlayKey::Red().pressed && PlayKey::Yellow().pressed;
		};
		break;
	case 16:
		m_judge = []() {
			return PlayKey::Blue().pressed && PlayKey::Red().pressed;
		};
		break;
	case 17:
		m_judge = []() {
			return PlayKey::Blue().pressed && PlayKey::Red().pressed&& PlayKey::Yellow().pressed;
		};
		break;
	default:
		m_judge = [&]() {
			return false;
		};
	};

};

//
void LongNote::perfect(Score& score)
{
	PlayStyle::Instance()->drawJudgeEffect(L"PERFECT", m_parent->getType());
	score.m_currentCombo++;
	score.m_judgeCount[Score::Perfect]++;
	m_isActive = false;
	m_parent->m_isActive = false;
}

void LongNote::miss(Score& score)
{
	PlayStyle::Instance()->drawJudgeEffect(L"MISS", m_parent->getType());
	score.m_currentCombo = 0;
	score.m_judgeCount[Score::Miss]++;
	m_isActive = false;
	m_parent->m_isActive = false;
}

bool LongNote::update(double& nowCount, double& countPerFrame, Score& score, Sound& sound)
{
	m_isActive = m_parent->m_isActive;//親のノーツの存在と同期

	if (!m_isActive||!m_parent->isFirstTap())
		return true;

	auto count = m_count  - nowCount;


	if (count <= 0)//ロングの終点
	{
		perfect(score);

		return true;
	}
	if (!(System::FrameCount() % 6))
	{
		if (m_type == 17)
		{
			PlayStyle::Instance()->drawTapEffect(7);
		}else
			PlayStyle::Instance()->drawTapEffect(m_parent->getType());
	}
	//オートプレイ----------------------
	if (AutoPlayManager::Instance()->m_autoPlay) 
	{

		return true;
	}
	//----------------------------------

	//親を押したのに途中で離した
	{
		if (!m_judge())
		{
			auto aCount = Abs(count);
			//パーフェクト範囲内での話はセーフ
			if (aCount <= JudgeRange(countPerFrame, Judge::Great))
				perfect(score);
			else
			miss(score);
		}
	}
	return true;
}
void LongNote::diffDraw(double count, float scrollRate)const
{
	PlayStyle::Instance()->draw(*this, count, scrollRate);
}
