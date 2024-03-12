#include <core/Data/Score/Score.hpp>
#include <commons/Constants.hpp>
#include <Siv3D.hpp>

namespace
{
	using namespace ct;
	void CalcLife(s3d::int32& life, s3d::int32& initLife, Score::Judge judge)
	{
#define MAPPING(val) {Score::##val, Constants::LifeRecovery::##val}
		static const std::unordered_map<Score::Judge, Constants::LifeRecovery> judgeRecoveyMap
		{
			MAPPING(Perfect),
			MAPPING(Great),
			MAPPING(Good),
			MAPPING(Miss),
		};
#undef MAPPING
		life += static_cast<s3d::int32>(judgeRecoveyMap.at(judge));
		if (life > 10000) {
			initLife -= (life - 10000);
			life = 10000;
		} else if (life < 0) {
			initLife -= life;
			life = 0;
		}
	}

	static const s3d::HashTable<Score::Judge, String> scoreMap
	{
		{ Score::Good,U"GOOD" },
		{ Score::Great,U"GREAT" },
		{ Score::Perfect,U"PERFECT" },
		{ Score::Miss, U"MISS" },
	};
}

namespace ct
{
	Score::Score() :
		m_currentCombo(0),
		m_maxCombo(0),
		m_judgeCount{ 0 },
		m_initLife(10000),
		m_life(10000)
	{
		m_lifeHistory.push_back(m_initLife);
	}
	void Score::add(Judge judge, s3d::int64 diff)
	{
		// ジャッジ数加算
		m_judgeCount[judge]++;
		m_judgeHistory.push_back(judge);
		// コンボ更新
		if (judge == Score::Miss) {
			m_currentCombo = 0;
		} else {
			++m_currentCombo;
		}
		m_maxCombo = s3d::Max(m_maxCombo, m_currentCombo);
		// ライフ更新
		::CalcLife(m_life, m_initLife, judge);
		m_lifeHistory.push_back(m_life);

		if (judge == Score::Great || judge == Score::Good) {
			if (diff > 0) {
				++m_fastCount;
			} else if (diff < 0) {
				++m_lateCount;
			}
		}
	}
	s3d::StringView JudgeStr(Score::Judge judge)
	{
		return scoreMap.at(judge);
	}
}
