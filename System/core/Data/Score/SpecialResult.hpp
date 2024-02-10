#pragma once
#include <Siv3D/Types.hpp>

namespace ct
{
	enum class SpecialResult : s3d::uint32
	{
		None,
		Full_Combo,
		All_Perfect,
	};
	inline bool operator > (SpecialResult& l, SpecialResult& r)
	{
		return static_cast<s3d::int32>(l) > static_cast<s3d::int32>(r);
	}
}