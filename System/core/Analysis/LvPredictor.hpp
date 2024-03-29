﻿#pragma once
#include <utils/Singleton/Singleton.hpp>
#include <Siv3D/String.hpp>
#include <Siv3D/Array.hpp>

namespace ct
{
    /// <summary>
    /// レベル予測器
    /// </summary>
    class LvPredictor : protected Singleton<LvPredictor>
    {
        friend class Singleton<LvPredictor>;
    public:

        /// <summary>
        /// 必要ならセットアップ
        /// </summary>
        /// <param name="path"></param>
        /// <returns></returns>
        static bool SetupIfNeed(s3d::FilePathView path = U"rating_lv_model.csv");

        /// <summary>
        /// 予測値の取得
        /// </summary>
        /// <param name="rating"></param>
        /// <returns></returns>
        static s3d::int32 Predict(s3d::uint64 rating);
    private:
        LvPredictor() = default;

        bool setup(s3d::FilePathView path);

        bool isValid() const
        {
            return m_isValid;
        }

        s3d::int32 predict(s3d::uint64 rating) const;
    private:
        bool m_isValid = false;
        s3d::Array<std::pair<s3d::int32, s3d::uint64>> m_thresholdMap;
    };
}