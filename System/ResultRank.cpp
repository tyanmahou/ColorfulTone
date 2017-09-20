#include"ResultRank.h"

String ResultRank::getRankTextureName(float clearRate)
{
	if (clearRate >= Rank::AAA)
	{
		return L"AAA";
	}
	else if (clearRate >= Rank::AA)
	{
		return L"AA";
	}
	else if (clearRate >= Rank::A)
	{
		return L"A";
	}
	else if (clearRate >= Rank::B)
	{
		return L"B";
	}
	else if (clearRate >= Rank::C)
	{
		return L"C";
	}
	else if (clearRate >= Rank::D)
	{
		return L"D";
	}
	else if (clearRate >= Rank::E)
	{
		return L"E";
	}
	
	return L"F";
}

float ResultRank::calcClearRate(const Score&score, int totalNotes)
{
	float clearRate;
	auto& judges = score.m_judgeCount;
	if (totalNotes != 0)
		clearRate = (judges[Score::Perfect] + judges[Score::Great] * 0.7f + judges[Score::Good] * 0.5f) / static_cast<float>(totalNotes);
	else
		clearRate = 1.0;
	int tmpRate = clearRate * 10000;
	clearRate = tmpRate / 100.0f;
	return clearRate;

}

float ResultRank::calcClearRateAsDownType(const Score & score, int totalNotes)
{
	float clearRate;
	auto& judges = score.m_judgeCount;
	if (totalNotes != 0)
		clearRate = 1.0-(judges[Score::Great] * 0.3f + judges[Score::Good] * 0.5f+ judges[Score::Miss] * 1) / static_cast<float>(totalNotes);
	else
		clearRate = 1.0;
	int tmpRate = clearRate * 10000;
	clearRate = tmpRate / 100.0f;
	return clearRate;
}

float ResultRank::calcLifeRate(const Score & score, float& initRate)
{
//	const float prevRate = initRate;
	//—¦
	constexpr float Perfect = 0.05f;
	constexpr float Great = -0.15f;
	constexpr float Good = -1.0f;
	constexpr float Miss = -5.0f;


	float life=0;
	auto& judges = score.m_judgeCount;
	const float slife = initRate+(judges[Score::Perfect]* Perfect + judges[Score::Great] * Great + judges[Score::Good] * Good+ judges[Score::Miss] * Miss);
	int tmpRate = slife * 100;
	life = tmpRate / 100.0f;
	if (life > 100)
	{
		initRate -= (slife - 100);
		life = 100;
	}
	else if (life < 0)
		life = 0;

	return life;
}
