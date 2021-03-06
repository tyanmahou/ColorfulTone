#include"ResultRank.h"

String ResultRank::GetRankTextureName(float clearRate)
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

float ResultRank::CalcBaseRate(const std::array<uint32, Score::TERM>& judges, uint32 total)
{
	if (total == 0)
	{
		return 1.0f;
	}
	return (judges[Score::Perfect] + judges[Score::Great] * 0.7f + judges[Score::Good] * 0.5f) / static_cast<float>(total);
}

float ResultRank::CalcClearRate(const Score&score, int totalNotes)
{
	float clearRate;
	auto& judges = score.m_judgeCount;
	clearRate = ResultRank::CalcBaseRate(judges, static_cast<uint32>(totalNotes));
	int tmpRate = clearRate * 10000;
	clearRate = tmpRate / 100.0f;
	return clearRate;
}

float ResultRank::CalcClearRateAsDownType(const Score & score, int totalNotes)
{
	float clearRate;
	auto& judges = score.m_judgeCount;
	if (totalNotes != 0)
		clearRate = 1.0 - (judges[Score::Great] * 0.3f + judges[Score::Good] * 0.5f + judges[Score::Miss] * 1) / static_cast<float>(totalNotes);
	else
		clearRate = 1.0;
	int tmpRate = clearRate * 10000;
	clearRate = tmpRate / 100.0f;
	return clearRate;
}

float ResultRank::CalcLifeRate(const Score & score)
{
	//率
	return static_cast<float>(score.m_life) / 100.0f;
}

ScoreModel ResultRank::CalcScore(const Score & score, int totalNotes)
{
	ScoreModel ret;
	const auto& judges = score.m_judgeCount;
	//ＡＰ処理
	if (judges[Score::Perfect] == totalNotes)
	{
		ret.specialResult = SpecialResult::All_Perfect;
	}
	//フルコン処理
	else if (score.m_currentCombo == totalNotes)
	{
		ret.specialResult = SpecialResult::Full_Combo;
	}

	ret.clearRate = ResultRank::CalcClearRate(score, totalNotes);

	ret.isClear = ret.clearRate >= 80;

	return ret;
}
