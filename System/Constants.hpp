#pragma once
#include<Siv3D/Fwd.hpp>
namespace Constants
{
	constexpr double JacketWidth = 350;  // 曲情報の幅
	constexpr double JacketCenter = 220; // 曲情報の中心

	// ライフ回復量
	enum class LifeRecovery: s3d::int32
	{
		Perfect = 5,
		Great = -15,
		Good = -100,
		Miss = -500,
	};

	namespace Stancil
	{
		constexpr s3d::uint8 HighSpeedMin = 3;
		constexpr s3d::uint8 HighSpeedMax = 4;

		constexpr s3d::uint8 BGJacket = 10;
	}
}