#pragma once

#include"GenreData.h"

class GenreManager final
{
private:
	Array<GenreData> m_refiners;

	bool add(const GenreType type, const String& name, const RefinerType& refiner, int lv);
	void clear();
	void sort();

	static GenreManager& Instance()
	{
		static GenreManager instance;
		return instance;
	}
public:
	static bool Add(const GenreType type, const String& name, const RefinerType& refiner, int lv = 0)
	{
		return Instance().add(type, name, refiner, lv);
	}
	static void Sort()
	{
		Instance().sort();
	}
	static void Clear()
	{
		Instance().clear();
	}
	static size_t Size()
	{
		return Instance().m_refiners.size();
	}
	static const Array<GenreData>& GetGenreDates()
	{
		return Instance().m_refiners;
	}
	static RefinerType GetRefiner(size_t index)
	{
		return Instance().m_refiners[index].m_refiner;
	}
};