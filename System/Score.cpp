#include"Score.h";
#include"Constants.hpp"
#include<Siv3D/Math.hpp>
#include<unordered_map>

bool operator > (SpecialResult& l, SpecialResult& r)
{
	return static_cast<unsigned int>(l) > static_cast<unsigned int>(r);
}

Score::Score() :
	m_currentCombo(0),
	m_maxCombo(0),
	m_judgeCount{ 0 },
	m_initLife(10000),
	m_life(10000)
{
	m_lifeHistory.push_back(m_initLife);
}

namespace
{
	void CalcLife(s3d::int32& life, s3d::int32& initLife, Score::Judge judge)
	{
		s3d::int32 ret = 0;
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
		if (life > 10000)
		{
			initLife -= (life - 10000);
			life = 10000;
		}
		else if (life < 0)
		{
			initLife -= life;
			life = 0;
		}
	}
}
void Score::add(Judge judge)
{
	// ジャッジ数加算
	m_judgeCount[judge]++;
	m_judgeHistory.push_back(judge);
	// コンボ更新
	if (judge == Score::Miss)
	{
		m_currentCombo = 0;
	}
	else
	{
		++m_currentCombo;
	}
	m_maxCombo = s3d::Max(m_maxCombo, m_currentCombo);
	// ライフ更新
	::CalcLife(m_life, m_initLife, judge);
	m_lifeHistory.push_back(m_life);
}
