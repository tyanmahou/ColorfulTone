#pragma once

namespace ct
{
	enum class SpecialResult : unsigned int
	{
		None,
		Full_Combo,
		All_Perfect,
	};
	inline bool operator > (SpecialResult& l, SpecialResult& r)
	{
		return static_cast<unsigned int>(l) > static_cast<unsigned int>(r);
	}
}