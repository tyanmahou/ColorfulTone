#pragma once
#include <Siv3D/Types.hpp>
#include <Siv3D/InputGroups.hpp>
#include <Siv3D/Scene.hpp>

namespace ct
{
	/// <summary>
	/// 加速設定
	/// </summary>
	struct AccelPressedDesc
	{
		static AccelPressedDesc CreateDefault();

		s3d::int32 timeMillisec = 5000;
		s3d::int32 waitMillisec = 500;
		s3d::int32 frameMin = 1;
		s3d::int32 frameMax = 10;

		bool isLongPress(const s3d::Duration& pressedDuration) const;
		bool isLongPress(s3d::int32 pressedDuration) const;
		s3d::int32 intervalFrame(s3d::int32 pressedDuration) const;
		s3d::int32 intervalFrame(const s3d::Duration& pressedDuration) const;
	};
    class InputUtil
    {
	public:
		static bool AccelPressed(const s3d::InputGroup& key, const AccelPressedDesc& desc = AccelPressedDesc::CreateDefault());
    };
}