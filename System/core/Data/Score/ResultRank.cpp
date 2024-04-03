#include <core/Data/Score/ResultRank.hpp>
#include <Siv3D.hpp>

namespace ct
{
	String ResultRank::GetRankTextureName(float clearRate)
	{
		s3d::int32 r = static_cast<s3d::int32>(clearRate);
		if (r >= Rank::AAA) {
			return U"AAA";
		} else if (r >= Rank::AA) {
			return U"AA";
		} else if (r >= Rank::A) {
			return U"A";
		} else if (r >= Rank::B) {
			return U"B";
		} else if (r >= Rank::C) {
			return U"C";
		} else if (r >= Rank::D) {
			return U"D";
		} else if (r >= Rank::E) {
			return U"E";
		}

		return U"F";
	}

	float ResultRank::CalcBaseRate(const std::array<size_t, Score::TERM>& judges, size_t total)
	{
		if (total == 0) {
			return 1.0f;
		}
		return (judges[Score::Perfect] + judges[Score::Great] * 0.7f + judges[Score::Good] * 0.5f) / static_cast<float>(total);
	}

	float ResultRank::CalcClearRate(const Score& score, size_t totalNotes)
	{
		float clearRate;
		auto& judges = score.m_judgeCount;
		clearRate = ResultRank::CalcBaseRate(judges, totalNotes);
		int32 tmpRate = static_cast<int32>(clearRate * 10000);
		clearRate = tmpRate / 100.0f;
		return clearRate;
	}

	float ResultRank::CalcClearRateAsDownType(const Score& score, size_t totalNotes)
	{
		float clearRate;
		auto& judges = score.m_judgeCount;
		if (totalNotes != 0)
			clearRate = 1.0f - (judges[Score::Great] * 0.3f + judges[Score::Good] * 0.5f + judges[Score::Miss] * 1) / static_cast<float>(totalNotes);
		else
			clearRate = 1.0f;
		int32 tmpRate = static_cast<int32>(clearRate * 10000);
		clearRate = tmpRate / 100.0f;
		return clearRate;
	}

	float ResultRank::CalcLifeRate(const Score& score)
	{
		//率
		return static_cast<float>(score.m_life) / 100.0f;
	}

	ScoreModel ResultRank::CalcScore(const Score& score, size_t totalNotes)
	{
		ScoreModel ret;
		const auto& judges = score.m_judgeCount;
		//ＡＰ処理
		if (judges[Score::Perfect] == totalNotes) {
			ret.specialResult = SpecialResult::All_Perfect;
		}
		//フルコン処理
		else if (score.m_currentCombo == totalNotes) {
			ret.specialResult = SpecialResult::Full_Combo;
		}

		ret.clearRate = ResultRank::CalcClearRate(score, totalNotes);

		ret.isClear = ret.clearRate >= 80;

		if (score.judgeCountTotal() == totalNotes) {
			// 最後までプレイしているかチェック

			ret.isLifeClear = score.m_life > 0;
			ret.gauge = score.m_gauge;
		}
		return ret;
	}

	float ResultRank::ToRate(Rank rank)
	{
		return static_cast<float>(rank);
	}

	float ResultRank::ToRate(const String& rank)
	{
		Rank r = Rank::F;
		if (rank == U"AAA") {
			r = Rank::AAA;
		} else if (rank == U"AA") {
			r = Rank::AA;
		} else if (rank == U"A") {
			r = Rank::A;
		} else if (rank == U"B") {
			r = Rank::B;
		} else if (rank == U"C") {
			r = Rank::D;
		} else if (rank == U"D") {
			r = Rank::D;
		} else if (rank == U"E") {
			r = Rank::E;
		} else if (rank == U"F") {
			r = Rank::F;
		}
		return ToRate(r);
	}

}
