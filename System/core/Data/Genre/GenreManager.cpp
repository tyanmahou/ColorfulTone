#include <core/Data/Genre/GenreManager.hpp>

namespace ct
{
	bool GenreManager::add(const GenreType type, const s3d::String& name, const RefinerType& refiner, s3d::int32 lv)
	{
		for (auto&& elm : m_refiners) {
			if (name == elm.m_name)
				return false;
		}

		m_refiners.emplace_back(type, name, refiner, lv);

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
}

