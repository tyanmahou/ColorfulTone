#pragma once
#include <Siv3D/Types.hpp>
#include <Siv3D/InputGroups.hpp>
#include <Siv3D/Scene.hpp>

namespace ct
{
    class InputUtil
    {
	public:
		template<class KeyType>
		static auto AccelPressed(const KeyType& key, s3d::int32 timeMillisec = 5000, s3d::int32 waitMillisec = 500)
			-> decltype(key.down(), key.pressedDuration(), true)
		{
			const s3d::int32 duration = s3d::DurationCast<s3d::Milliseconds>(key.pressedDuration()).count();
			const s3d::int32 time = timeMillisec - waitMillisec;
			const s3d::int32 interval = time <= 0 ? 1 : 10 - 9 * s3d::Min(time, (duration - waitMillisec)) / time;

			const s3d::int32 frame = static_cast<s3d::int32>(s3d::Scene::Time() * 60);
			return key.down() || duration >= waitMillisec && frame % interval == 0;
		}
    };
}