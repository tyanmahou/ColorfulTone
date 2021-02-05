#pragma once
#include"CourseData.h"
#include"Singleton.h"

//コースモードのジャンル
class CourseGenre
{
public:
	using RefinerType = std::function<bool(CourseData&)>;
private:
	RefinerType m_refiner;
	String m_name;
public:
	CourseGenre(String name):
		m_name(name),
		m_refiner([name](CourseData& course){
		return course.getGenre() == name;
	})
	{}
	const String& getName()const
	{
		return m_name;
	}
	const RefinerType& getRefiner()const
	{
		return m_refiner;
	}
};

class CourseGenreManager :protected Singleton<CourseGenreManager>
{
	friend class Singleton<CourseGenreManager>;
private:
	Array<CourseGenre> m_courseGenres;
	const Array<CourseGenre>& getGenre()const
	{
		return m_courseGenres;
	}
	bool add(const String& genreName)
	{
		for (auto&&elm : m_courseGenres)
		{
			if (genreName == elm.getName())
				return false;
		}

		m_courseGenres.emplace_back(genreName);

		return true;
	}
	void clear()
	{
		m_courseGenres.clear();
	}
	CourseGenreManager() = default;
public:
	static void Clear()
	{
		CourseGenreManager::Instance()->clear();
	}
	static bool Add(const String& genreName)
	{
		return CourseGenreManager::Instance()->add(genreName);
	}
	static const Array<CourseGenre>& Genres()
	{
		return CourseGenreManager::Instance()->getGenre();
	}
	static CourseGenre::RefinerType GetRefiner(size_t index)
	{
		return Instance()->m_courseGenres[index].getRefiner();
	}
};