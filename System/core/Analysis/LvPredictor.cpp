#include <core/Analysis/LvPredictor.hpp>
#include <commons/Secret/Secret.hpp>
#include <core/Data/NotesData/StarLv.hpp>
#include <Siv3D.hpp>

namespace ct
{
    constexpr int32 maxLv = 16;
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
        size_t rows = csv.rows();
        for (size_t i = 0; i < rows; ++i) {
            data.emplace_back(
                csv.getOpt<double>(i, 0).value_or(0),
                csv.getOpt<double>(i, 1).value_or(0)
            );
        }
        if (data.isEmpty()) {
            return false;
        }
        m_linearRegression.fit(data);
        m_isValid = true;
    }
    s3d::int32 LvPredictor::predict(s3d::uint64 rating) const
    {
        double flv = m_linearRegression.predict(static_cast<double>(rating));
        return Clamp(static_cast<int32>(s3d::Round(flv)), 0, maxLv);
    }
}
