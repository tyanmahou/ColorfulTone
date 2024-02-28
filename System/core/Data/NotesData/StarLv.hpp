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
		OneWhite,   // ☆　　譜面
		One,        // ★　　譜面
		TwoWhite,   // ☆☆　譜面
		Two,        // ★★　譜面
		ThreeWhite, // ☆☆☆譜面
		Three,      // ★★★譜面
	};

	inline s3d::String ToStr(StarLv kind)
	{
		switch (kind) {
		case StarLv::None:
			return U"";
		case StarLv::OneWhite:
			return U"☆";
		case StarLv::One:
			return U"★";
		case StarLv::TwoWhite:
			return U"☆☆";
		case StarLv::Two:
			return U"★★";
		case StarLv::ThreeWhite:
			return U"☆☆☆";
		case StarLv::Three:
			return U"★★★";
		default:
			return U"";
		}
	}
	inline StarLv ParseStarLv(s3d::StringView str)
	{
		if (str == U"☆") {
			return StarLv::OneWhite;
		}
		if (str == U"★") {
			return StarLv::One;
		}
		if (str == U"☆☆") {
			return StarLv::TwoWhite;
		}
		if (str == U"★★") {
			return StarLv::Two;
		}
		if (str == U"☆☆☆") {
			return StarLv::ThreeWhite;
		}
		if (str == U"★★★") {
			return StarLv::Three;
		}
		return StarLv::None;
	}
}
