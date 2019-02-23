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
		A=90,
		AA = 95,
		AAA = 97,
	};
public:
	static 	String GetRankTextureName(float clearRate);
	static float CalcClearRate(const Score&score, int totalNotes);
	static float CalcClearRateAsDownType(const Score&score, int totalNotes);
	static float CalcLifeRate(const Score&score, float& initRate);

	static ScoreModel CalcScore(const Score& score, int totalNotes);
private:
	ResultRank();
};