#include <core/Play/Score/PlayingScore.hpp>
#include <core/Play/LifeGauge/LifeGauge.hpp>
#include <commons/Constants.hpp>
#include <Siv3D.hpp>

namespace
{
	using namespace ct;
	s3d::int32 GetRecovery(ScoreJudge judge, LifeGaugeKind gaugeKind)
	{
		auto guage = LifeRecoverySet::FromKind(gaugeKind);
		switch (judge) {
		case ScoreJudge::Perfect:
			return guage.perfect;
		case ScoreJudge::Great:
			return guage.great;
		case ScoreJudge::Good:
			return guage.good;
		case ScoreJudge::Miss:
			return guage.miss;
		default:
			return 0;
		}
	}
	void CalcLife(s3d::int32& life, s3d::int32& initLife, ScoreJudge judge, LifeGaugeKind gaugeKind)
	{
		if (life <= 0) {
			return;
		}
		life += ::GetRecovery(judge, gaugeKind);
		if (life > 10000) {
			initLife -= (life - 10000);
			life = 10000;
		} else if (life < 0) {
			initLife -= life;
			life = 0;
		}
	}
}

namespace ct
{
	PlayingScore::PlayingScore() :
		PlayingScore(LifeGaugeKind::Normal)
	{
	}
	PlayingScore::PlayingScore(LifeGaugeKind gauge) :
		m_gauge(gauge),
		m_currentCombo(0),
		m_maxCombo(0),
		m_currentTotalCombo(0),
		m_maxTotalCombo(0),
		m_judgeCount{ 0 },
		m_initLife(10000),
		m_life(10000)
	{
		m_lifeHistory.push_back(m_initLife);
	}
	void PlayingScore::add(ScoreJudge judge, s3d::int64 diff)
	{
		// ジャッジ数加算
		m_judgeCount[judge]++;
		m_judgeHistory.push_back(judge);
		// コンボ更新
		if (judge == ScoreJudge::Miss) {
			m_currentCombo = 0;
			m_currentTotalCombo = 0;
		} else {
			++m_currentCombo;
			++m_currentTotalCombo;
		}
		m_maxCombo = s3d::Max(m_maxCombo, m_currentCombo);
		m_maxTotalCombo = s3d::Max(m_maxTotalCombo, m_currentTotalCombo);

		// ライフ更新
		::CalcLife(m_life, m_initLife, judge, m_gauge);
		m_lifeHistory.push_back(m_life);

		if (judge == ScoreJudge::Great || judge == ScoreJudge::Good) {
			if (diff > 0) {
				++m_fastCount;
			} else if (diff < 0) {
				++m_lateCount;
			}
		}
	}
	size_t PlayingScore::judgeCountTotal() const
	{
		size_t ret = 0;
		for (size_t c : m_judgeCount) {
			ret += c;
		}
		return ret;
	}
}
