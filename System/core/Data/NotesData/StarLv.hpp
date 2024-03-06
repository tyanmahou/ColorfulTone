#pragma once
#include <Siv3D/String.hpp>

namespace ct
{
	/// <summary>
	/// ★レベル
	/// </summary>
	enum class StarLv
	{
		None,
		WhiteOne,   // ☆　　譜面
		WhiteTwo,   // ☆☆　譜面
		WhiteThree, // ☆☆☆譜面
		BlackOne,   // ★　　譜面
		BlackTwo,   // ★★　譜面
		BlackThree, // ★★★譜面
	};

	inline s3d::String ToStr(StarLv kind)
	{
		switch (kind) {
		case StarLv::None:
			return U"";
		case StarLv::WhiteOne:
			return U"☆";
		case StarLv::WhiteTwo:
			return U"☆☆";
		case StarLv::WhiteThree:
			return U"☆☆☆";
		case StarLv::BlackOne:
			return U"★";
		case StarLv::BlackTwo:
			return U"★★";
		case StarLv::BlackThree:
			return U"★★★";
		default:
			return U"";
		}
	}
	inline StarLv ParseStarLv(s3d::StringView str)
	{
		if (str == U"☆") {
			return StarLv::WhiteOne;
		}
		if (str == U"☆☆") {
			return StarLv::WhiteTwo;
		}
		if (str == U"☆☆☆") {
			return StarLv::WhiteThree;
		}
		if (str == U"★") {
			return StarLv::BlackOne;
		}
		if (str == U"★★") {
			return StarLv::BlackTwo;
		}
		if (str == U"★★★") {
			return StarLv::BlackThree;
		}
		return StarLv::None;
	}

	inline bool IsWhiteStar(StarLv kind)
	{
		switch (kind) {
		case StarLv::WhiteOne:
		case StarLv::WhiteTwo:
		case StarLv::WhiteThree:
			return true;
		default:
			return false;
		}
	}
	inline bool IsBlackStar(StarLv kind)
	{
		switch (kind) {
		case StarLv::BlackOne:
		case StarLv::BlackTwo:
		case StarLv::BlackThree:
			return true;
		default:
			return false;
		}
	}
	inline s3d::int32 StarCount(StarLv kind)
	{
		switch (kind) {
		case StarLv::WhiteOne:
		case StarLv::BlackOne:
			return 1;
		case StarLv::WhiteTwo:
		case StarLv::BlackTwo:
			return 2;
		case StarLv::WhiteThree:
		case StarLv::BlackThree:
			return 3;
		default:
			return 0;
		}
	}
}
