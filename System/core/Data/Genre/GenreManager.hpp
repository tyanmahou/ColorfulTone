#pragma once
#include <core/Data/Genre/GenreData.hpp>

namespace ct
{
    class GenreManager final
    {
    private:
        s3d::Array<GenreData> m_refiners;

        bool add(const GenreData& genre);
        void clear();
        void sort();
        void reflesh(const s3d::Array<GenreData>& newData);
        static GenreManager& Instance()
        {
            static GenreManager instance;
            return instance;
        }
    public:
        static void Reflesh(const s3d::Array<GenreData>& newData)
        {
            Instance().reflesh(newData);
        }
        static void Clear()
        {
            Instance().clear();
        }
        static size_t Size()
        {
            return Instance().m_refiners.size();
        }
        static const s3d::Array<GenreData>& GetGenreDates()
        {
            return Instance().m_refiners;
        }
        static RefinerType GetRefiner(size_t index)
        {
            return Instance().m_refiners[index].getRefiner();
        }
    };
}
