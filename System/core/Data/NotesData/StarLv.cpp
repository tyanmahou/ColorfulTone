#include <core/Data/NotesData/StarLv.hpp>

namespace ct
{
	s3d::StringView ToStr(StarLv kind)
	{
		switch (kind) {
		case StarLv::None:
			return U"";
		case StarLv::AsteOne:
			return U"＊";
		case StarLv::AsteTwo:
			return U"＊＊";
		case StarLv::AsteThree:
			return U"＊＊＊";
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
		case StarLv::Other:
			return U"？";
		default:
			return U"";
		}
	}
	StarLv ParseStarLv(s3d::StringView str)
	{
		if (str == U"＊") {
			return StarLv::AsteOne;
		}
		if (str == U"＊＊") {
			return StarLv::AsteTwo;
		}
		if (str == U"＊＊＊") {
			return StarLv::AsteThree;
		}
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
		if (str == U"？") {
			return StarLv::Other;
		}
		return StarLv::None;
	}
	bool IsAsteStar(StarLv kind)
	{
		switch (kind) {
		case StarLv::AsteOne:
		case StarLv::AsteTwo:
		case StarLv::AsteThree:
			return true;
		default:
			return false;
		}
	}
	bool IsWhiteStar(StarLv kind)
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
	bool IsBlackStar(StarLv kind)
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
	s3d::int32 StarCount(StarLv kind)
	{
		switch (kind) {
		case StarLv::AsteOne:
		case StarLv::WhiteOne:
		case StarLv::BlackOne:
		case StarLv::Other:
			return 1;
		case StarLv::AsteTwo:
		case StarLv::WhiteTwo:
		case StarLv::BlackTwo:
			return 2;
		case StarLv::AsteThree:
		case StarLv::WhiteThree:
		case StarLv::BlackThree:
			return 3;
		default:
			return 0;
		}
	}

	s3d::StringView ToBaseStarStr(StarLv kind)
	{
		if (IsBlackStar(kind)) {
			return U"★";
		}
		if (IsWhiteStar(kind)) {
			return U"☆";
		}
		if (IsAsteStar(kind)) {
			return U"＊";
		}
		if (kind == StarLv::Other) {
			return U"？";
		}
		return U"";
	}
}