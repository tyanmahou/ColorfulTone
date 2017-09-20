#pragma once

#include"GenreData.h"

class GenreManager 
{
public:
	static Array<GenreData> m_refiners;
	static bool Add(const GenreType type, const String& name, const RefinerType& refiner, int lv = 0)
	{
		for (auto&&elm : m_refiners)
		{
			if (name == elm.m_name)
				return false;
		}

		m_refiners.emplace_back(type,name, refiner,lv);

		return true;
	}
	static void Sort()
	{
		sort(m_refiners.begin(),m_refiners.end());
	}
};