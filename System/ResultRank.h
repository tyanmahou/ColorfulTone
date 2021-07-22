#pragma once
#include<Siv3D.hpp>
#include"Score.h"
class ResultRank
{
	enum Rank:unsigned int
	{
		F = 0,
		E = 50,
		D = 60,
		C = 70,
		B = 80,
		A = 90,
		AA = 95,
		AAA = 97,
	};
public:
	static 	String GetRankTextureName(float clearRate);
	static float CalcBaseRate(const std::array<uint32, Score::TERM>& judges, uint32 total);
	static float CalcClearRate(const Score&score, int totalNotes);
	static float CalcClearRateAsDownType(const Score&score, int totalNotes);
	static float CalcLifeRate(const Score&score);

	static ScoreModel CalcScore(const Score& score, int totalNotes);

	static float ToRate(Rank rank);
	static float ToRate(const String& rank);
private:
	ResultRank();
};