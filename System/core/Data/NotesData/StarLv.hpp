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
		AsteOne,    // ＊　　譜面
		AsteTwo,    // ＊＊　譜面
		AsteThree,  // ＊＊＊譜面
		WhiteOne,   // ☆　　譜面
		WhiteTwo,   // ☆☆　譜面
		WhiteThree, // ☆☆☆譜面
		BlackOne,   // ★　　譜面
		BlackTwo,   // ★★　譜面
		BlackThree, // ★★★譜面
		Other,      // ？    譜面
	};

	[[nodiscard]] s3d::StringView ToStr(StarLv kind);
	[[nodiscard]] s3d::StringView ToBaseStarStr(StarLv kind);
	[[nodiscard]] s3d::int32 StarCount(StarLv kind);

	[[nodiscard]] StarLv ParseStarLv(s3d::StringView str);

	[[nodiscard]] bool IsAsteStar(StarLv kind);
	[[nodiscard]] bool IsWhiteStar(StarLv kind);
	[[nodiscard]] bool IsBlackStar(StarLv kind);
}
