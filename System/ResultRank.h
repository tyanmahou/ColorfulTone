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
	static 	String getRankTextureName(float clearRate);
	static float calcClearRate(const Score&score, int totalNotes);
	static float calcClearRateAsDownType(const Score&score, int totalNotes);
	static float calcLifeRate(const Score&score, float& initRate);

private:
	ResultRank();
};