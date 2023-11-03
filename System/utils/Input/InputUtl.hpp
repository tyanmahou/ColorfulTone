#pragma once
#include <Siv3D/Types.hpp>
#include <Siv3D/InputGroups.hpp>
#include <Siv3D/Scene.hpp>
#include <utils/Fps/Fps.hpp>

namespace ct
{
    class InputUtil
    {
	public:
		template<class KeyType>
		static auto AccelPressed(const KeyType& key, s3d::int32 timeMillisec = 2500, s3d::int32 waitMillisec = 500, const Fps& fps = 60_fps)
			-> decltype(key.down(), key.pressedDuration(), true)
		{
			double deltaFrame = fps.frame(s3d::Scene::DeltaTime());
			if (deltaFrame == 0) {
				return key.down();
			}
			const s3d::int32 duration = s3d::DurationCast<s3d::Milliseconds>(key.pressedDuration()).count();
			const s3d::int32 time = timeMillisec - waitMillisec;

			const s3d::int32 frame = s3d::Max(1, static_cast<s3d::int32>(s3d::Round(1.0 / deltaFrame)));

			const s3d::int32 interval = (time <= 0 ? 5 : 10 - 5 * s3d::Min(time, (duration - waitMillisec)) / time) * frame;
			return key.down() || duration >= waitMillisec && s3d::Scene::FrameCount() % interval == 0;
		}
    };
}