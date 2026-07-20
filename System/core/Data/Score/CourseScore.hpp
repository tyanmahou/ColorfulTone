#pragma once
#include <Siv3D/Fwd.hpp>
#include <core/Play/LifeGauge/LifeGaugeKind.hpp>

namespace ct
{
	enum class CourseSpecialResult : s3d::uint8
	{
		None = 0,
		RankAAA = 1,
		AP = 2
	};
	enum class CoursePassKind
	{
		None,
		Fail,
		KariPass,
		Pass,
	};
	struct CourseScore
	{
		bool isClear = false;
		CourseSpecialResult special = CourseSpecialResult::None;
		float totalRate = 0.0;
		float life = 0.0;

		bool isLifeClear = false;
		LifeGaugeKind gauge = LifeGaugeKind::None;

		bool isMainClear() const
		{
			return isClear && gauge >= LifeGaugeKind::Normal;
		}
		void init(LifeGaugeKind _gauge)
		{
			this->life = 100.0;
			this->gauge = _gauge;
		}
		CoursePassKind passKind() const
		{
			if (isMainClear()) {
				return CoursePassKind::Pass;
			} else if (isClear) {
				return CoursePassKind::KariPass;
			} else if (totalRate > 0 && life == 0.0f) {
				return CoursePassKind::Fail;
			} else {
				return CoursePassKind::None;
			}
		}
	};
}