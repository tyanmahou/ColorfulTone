#pragma once

#include <Siv3D/Statistics.hpp>
#include <concepts>

namespace ct
{
    class StatisticsUtil
    {
    public:
        template<class T>
        static T Min(const s3d::Array<T>& ar)
        {
            return s3d::Statistics::Min(ar.begin(), ar.end()).value_or(T{});
        }
        template<class T>
        static T Max(const s3d::Array<T>& ar)
        {
            return s3d::Statistics::Max(ar.begin(), ar.end()).value_or(T{});
        }
        template<class T, class U = double>
        static U Mean(const s3d::Array<T>& ar)
        {
            return static_cast<U>(s3d::Statistics::Mean(ar.begin(), ar.end()));
        }
        template<class T, class U = double>
        static U Median(const s3d::Array<T>& ar)
        {
            return static_cast<U>(s3d::Statistics::Median(ar.begin(), ar.end()).value_or(0));
        }
    };
}