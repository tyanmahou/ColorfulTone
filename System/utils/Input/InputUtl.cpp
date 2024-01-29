#include <utils/Input/InputUtl.hpp>
#include <utils/Addon/IntervalCounter.hpp>
#include <Siv3D.hpp>

namespace ct
{
	AccelPressedDesc AccelPressedDesc::CreateDefault()
	{
		return {
			.timeMillisec = 3000,
			.waitMillisec = 500,
			.frameMin = 4,
			.frameMax = 10
		};
	}
	bool AccelPressedDesc::isLongPress(const s3d::Duration& pressedDuration) const
	{
		const s3d::int32 duration = static_cast<s3d::int32>(s3d::DurationCast<s3d::Milliseconds>(pressedDuration).count());
		return this->isLongPress(duration);
	}
	bool AccelPressedDesc::isLongPress(s3d::int32 pressedDuration) const
	{
		const s3d::int32 interval = intervalFrame(pressedDuration);
		return pressedDuration >= waitMillisec && IntervalCounter::IsUpdatedEvery(interval);
	}
	s3d::int32 AccelPressedDesc::intervalFrame(s3d::int32 pressedDuration) const
	{
		const s3d::int32 time = timeMillisec - waitMillisec;
		const int32 interval = time <= 0 ? frameMin : frameMax - (frameMax - frameMin) * Min(time, (pressedDuration - waitMillisec)) / time;
		return interval;
	}
	s3d::int32 AccelPressedDesc::intervalFrame(const s3d::Duration& pressedDuration) const
	{
		const s3d::int32 duration = static_cast<s3d::int32>(s3d::DurationCast<s3d::Milliseconds>(pressedDuration).count());
		return this->intervalFrame(duration);
	}
    bool InputUtil::AccelPressed(const s3d::InputGroup& key, const AccelPressedDesc& desc)
    {
		return key.down() || desc.isLongPress(key.pressedDuration());
    }
}
