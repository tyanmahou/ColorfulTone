#include <core/Data/Genre/GenreManager.hpp>

namespace ct
{
	bool GenreManager::add(const GenreData& genre)
	{
		m_refiners.emplace_back(genre);
		return true;
	}

	void GenreManager::clear()
	{
		m_refiners.clear();
	}

	void GenreManager::sort()
	{
		std::sort(m_refiners.begin(), m_refiners.end());
	}
	void GenreManager::reflesh(const s3d::Array<GenreData>& newData)
	{
		for (const GenreData& g : newData) {
			add(g);
		}
		sort();
	}
}

