#pragma once
#include <core/Data/Score/CourseScore.hpp>

namespace ct
{
    struct CourseResult
    {
        CourseScore score;
		size_t rankAAACount = 0;
		size_t apCount = 0;

		size_t trackCount;
		bool isEnd = false;

		void init(LifeGaugeKind gauge, size_t _tackCount)
		{
			score = {};
			score.init(gauge);
			rankAAACount = 0;
			apCount = 0;

			trackCount = _tackCount;
			isEnd = false;
		}
		CoursePassKind passKind() const
		{
			return score.passKind();
		}
		bool isInvincibleGauge() const
		{
			return score.gauge == LifeGaugeKind::Invincible;
		}
		void updateScore(float addRate, float life)
		{
			score.life = life;

			score.totalRate += addRate;
			s3d::int32 tmpRate = static_cast<s3d::int32>(score.totalRate * 100.0f);
			score.totalRate = tmpRate / 100.0f;

			if (addRate >= 100.0f) {
				++apCount;
			}
			if (addRate >= 97.0f) {
				++rankAAACount;
			}
		}
		void end(bool isClear)
		{
			isEnd = true;
			if (isClear) {
				score.isClear = !this->isInvincibleGauge();
				score.isLifeClear = true;
				if (apCount >= trackCount) {
					score.special = CourseSpecialResult::AP;
				} else if (rankAAACount >= trackCount) {
					score.special = CourseSpecialResult::RankAAA;
				}
			}
		}
    };
}