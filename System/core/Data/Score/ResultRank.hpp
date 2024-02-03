#pragma once
#include <core/Data/Score/Score.hpp>

namespace ct
{
	class ResultRank
	{
		enum Rank : s3d::int32
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
		static s3d::String GetRankTextureName(float clearRate);
		static float CalcBaseRate(const std::array<uint32, Score::TERM>& judges, s3d::uint32 total);
		static float CalcClearRate(const Score& score, s3d::uint32 totalNotes);
		static float CalcClearRateAsDownType(const Score& score, s3d::uint32 totalNotes);
		static float CalcLifeRate(const Score& score);

		static ScoreModel CalcScore(const Score& score, s3d::uint32 totalNotes);

		static float ToRate(Rank rank);
		static float ToRate(const s3d::String& rank);
	private:
		ResultRank();
	};
}
