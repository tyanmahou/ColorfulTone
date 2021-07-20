#pragma once

/// <summary>
/// ★レベル
/// </summary>
enum class StarLv
{
	None,
	One,   // ★　譜面
	Two    // ★★譜面
};

inline String ToStr(StarLv kind)
{
	switch (kind) {
	case StarLv::None:
		return L"";
	case StarLv::One:
		return L"★";
	case StarLv::Two:
		return L"★★";
	default:
		return L"";
	}
}