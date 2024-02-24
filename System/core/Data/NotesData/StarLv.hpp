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
		One,   // ★　　譜面
		Two,   // ★★　譜面
		Three, // ★★★譜面
	};

	inline s3d::String ToStr(StarLv kind)
	{
		switch (kind) {
		case StarLv::None:
			return U"";
		case StarLv::One:
			return U"★";
		case StarLv::Two:
			return U"★★";
		case StarLv::Three:
			return U"★★★";
		default:
			return U"";
		}
	}
}
