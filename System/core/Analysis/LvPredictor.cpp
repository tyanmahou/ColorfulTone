#include <core/Analysis/LvPredictor.hpp>
#include <commons/Secret/Secret.hpp>
#include <core/Data/NotesData/StarLv.hpp>
#include <utils/Math/StatisticsUtil.hpp>
#include <Siv3D.hpp>

namespace ct
{
    constexpr int32 maxLv = 19;
    bool LvPredictor::SetupIfNeed(FilePathView path)
    {
        if (Instance()->isValid()) {
            return false;
        }
        return Instance()->setup(path);
    }
    s3d::int32 LvPredictor::Predict(s3d::uint64 rating)
    {
        return Instance()->predict(rating);
    }
    bool LvPredictor::setup(FilePathView path)
    {
        CSV csv;
        if (Network::IsConnected()) {
            MemoryWriter writer;
            auto result = SimpleHTTP::Load(Secret::RESOURCE_URL + path, writer);
            if (!result) {
                return false;
            }
            csv.load(MemoryReader(writer.getBlob()));
        } else {
            csv.load(path);
        }
        if (!csv) {
            return false;
        }
        Array<std::pair<double, double>> data;

        std::map<int32, Array<uint64>> ratingMap;
        size_t rows = csv.rows();
        for (size_t i = 0; i < rows; ++i) {
            auto rating = csv.getOpt<uint64>(i, 0);
            auto lv = csv.getOpt<int32>(i, 1);
            if (!lv || !rating) {
                continue;
            }
            ratingMap[*lv] << *rating;
        }
        s3d::Array<std::pair<s3d::int32, s3d::uint64>> meanMap;
        for (auto&& [lv, d] : ratingMap) {
            uint64 mean = StatisticsUtil::Mean<uint64, uint64>(d);
            meanMap.emplace_back(lv, mean);
        }
        if (meanMap.isEmpty()) {
            return false;
        }
        size_t n = meanMap.size();
        for (size_t index = 0; index < n; ++index) {
            s3d::uint64 prevMean = index > 0 ? meanMap[index - 1].second : 0;
            s3d::uint64 mean = meanMap[index].second;
            s3d::uint64 begin = index > 0 ? (prevMean + mean) / 2 : 0;

            m_thresholdMap.emplace_back(meanMap[index].first, begin);
        }
        m_isValid = true;
        return true;
    }
    s3d::int32 LvPredictor::predict(s3d::uint64 rating) const
    {
        for (auto [lv, threshold] : m_thresholdMap | std::ranges::views::reverse) {
            if (rating >= threshold) {
                return lv;
            }
        }
        return 0;
    }
}
