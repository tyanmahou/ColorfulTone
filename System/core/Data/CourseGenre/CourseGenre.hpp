#pragma once
#include <core/Data/CourseData/CourseData.hpp>
#include <utils/Singleton/DynamicSingleton.hpp>

namespace ct
{
    //コースモードのジャンル
    class CourseGenre
    {
    public:
        using RefinerType = std::function<bool(CourseData&)>;
    private:
        RefinerType m_refiner;
        s3d::String m_name;
    public:
        CourseGenre(s3d::String name) :
            m_name(name),
            m_refiner([name](CourseData& course) {
            return course.getGenre() == name;
                })
        {}
        const s3d::String& getName()const
        {
            return m_name;
        }
        const RefinerType& getRefiner()const
        {
            return m_refiner;
        }
    };

    class CourseGenreManager :protected DynamicSingleton<CourseGenreManager>
    {
        friend class DynamicSingleton<CourseGenreManager>;
    private:
        s3d::Array<CourseGenre> m_courseGenres;
        const s3d::Array<CourseGenre>& getGenre()const
        {
            return m_courseGenres;
        }
        bool add(const s3d::String& genreName)
        {
            for (auto&& elm : m_courseGenres) {
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
        static bool Add(const s3d::String& genreName)
        {
            return CourseGenreManager::Instance()->add(genreName);
        }
        static const s3d::Array<CourseGenre>& Genres()
        {
            return CourseGenreManager::Instance()->getGenre();
        }
        static CourseGenre::RefinerType GetRefiner(size_t index)
        {
            return Instance()->m_courseGenres[index].getRefiner();
        }
    };
}
