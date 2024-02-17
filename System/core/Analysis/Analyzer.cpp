#include <core/Analysis/Analyzer.hpp>
#include <utils/Math/StatisticsUtil.hpp>
#include <Siv3D.hpp>

namespace
{
    using namespace ct;

    double TypeFactor(const NoteEntity& note)
    {
        switch (note.type) {
        case 1:
        case 2:
        case 3:
            return 1.0;
        case 4:
        case 5:
        case 6:
            return 2.0;
        case 7:
            return 3.0;
        case 8:
            return 0.0;
        case 9:
            return 0.1;
        case 10:
            return 1.2 * note.interval / 8.0;
        case 11:
        case 12:
        case 13:
            return 1.2;
        case 14:
        case 15:
        case 16:
            return 2.4;
        case 17:
            return 3.6;
        case 18:
            return 0.9;
        default:
            return 0.0;
        }
    }

    int32 NoteTypeBit(NoteType type)
    {
        switch (type) {
        case 1:
        case 11:
            return 0b001;
        case 2:
        case 12:
            return 0b010;
        case 3:
        case 13:
            return 0b100;
        case 4:
        case 14:
            return 0b110;
        case 5:
        case 15:
            return 0b101;
        case 6:
        case 16:
            return 0b011;
        case 7:
        case 17:
            return 0b111;
        default:
            return 0b000;
        }
    }
}
namespace ct
{

    AnalyzeResult Analyzer::Analyze(const SheetMusic& sheet)
    {
        // 1秒間隔のスコア 反比例
        constexpr double BaseNoteRating = 1000.0;
        constexpr int64 NoteDiffThresholdMin = static_cast<int64>(44100.0 * 0.01);
        constexpr int64 NoteDiffThresholdMax = 44100 * 2;
        // ロング終点以外
        const auto notes = sheet.getNotes().filter([](const NoteEntity& e) {
            return e.type != 8;
        });

        Array<std::pair<int64, double>> notesRating;
        Array<double> speedDiff;
        int64 lastSample = 0;
        std::array<int64, 3> lastSampleBits{ 0,0,0 };

        notesRating.reserve(notes.size());
        for (size_t index = 0; index < notes.size(); ++index) {
            //if (notes[index].speed != notes[index - 1].speed) {
            //    double minSpeed = Min(notes[index].speed, notes[index - 1].speed);
            //    double maxSpeed = Max(notes[index].speed, notes[index - 1].speed);
            //    if (minSpeed != 0.0) {
            //        speedDiff.push_back(((minSpeed * maxSpeed <= 0) ? 2 : 1) * Abs(maxSpeed / minSpeed));
            //    } else {
            //        speedDiff.push_back(2);
            //    }
            //}
            int32 typebit = NoteTypeBit(notes[index].type);
            int64 nearSample = 0;
            if (typebit == 0) {
                nearSample = lastSample;
            } else {
                if ((typebit & 0b001) != 0) { // 赤
                    nearSample = Max(nearSample, lastSampleBits[0]);
                }
                if ((typebit & 0b010) != 0) { // 青
                    nearSample = Max(nearSample, lastSampleBits[1]);
                }
                if ((typebit & 0b100) != 0) { // 黄
                    nearSample = Max(nearSample, lastSampleBits[2]);
                }
            }

            int64 diff = Clamp(notes[index].sample - nearSample, NoteDiffThresholdMin, NoteDiffThresholdMax);
            if (diff <= 0) {
                // 無いはずだけどDiffがないなら
                notesRating.emplace_back(notes[index].sample, BaseNoteRating * TypeFactor(notes[index]));
            } else {
                // 間隔係数
                double jackFactor = Math::Log(1 / (static_cast<double>(diff) / 44100.0)) + 1.0;
                double score = BaseNoteRating * jackFactor * TypeFactor(notes[index]);
                notesRating.emplace_back(notes[index].sample, score);
            }
            // 最終タップ更新
            if ((typebit & 0b001) != 0) { // 赤
                lastSampleBits[0] = notes[index].sample;
            }
            if ((typebit & 0b010) != 0) { // 青
                lastSampleBits[1] = notes[index].sample;
            }
            if ((typebit & 0b100) != 0) { // 黄
                lastSampleBits[2] = notes[index].sample;
            }
            if (notes[index].type != 9) {
                // 白じゃなければ
                lastSample = notes[index].sample;
            }
        }

        // 小節事にレーティング 計算
        Array<double> barRatings;
        {
            const auto& bars = sheet.getBars();
            size_t notesIndex = 0;
            for (size_t barIndex = 0; barIndex < bars.size(); ++barIndex) {
                s3d::int64 nextBarSample = (barIndex + 1) < bars.size() ? bars[barIndex + 1].sample : sheet.getOffsetedTotalSample();

                double sum = 0;
                // ノーツレート
                while (notesIndex < notesRating.size() && notesRating[notesIndex].first < nextBarSample) {
                    sum += notesRating[notesIndex].second;
                    ++notesIndex;
                }
                if (sum <= 0) {
                    // 何もないなら含めない
                    continue;
                }
                int64 sample = nextBarSample - bars[barIndex].sample;
                if (sample <= 0) {
                    continue;
                }
                double sec = static_cast<double>(sample) / 44100.0;
                barRatings.push_back(sum / sec);
            }
        }
        // 平均レート
        double meanRating = StatisticsUtil::Mean(barRatings);
        // 最大局所レート
        double maxRating = StatisticsUtil::Max(barRatings);
        // 中央局所レート
        double medianRating = StatisticsUtil::Median(barRatings);

        // レート
        const double ratingResult = meanRating * 0.5 + medianRating * 0.4 + maxRating * 0.1;

        //// 停止レート 1個につき
        //constexpr double BaseStopRating = 100.0;
        //double stopRating = 0;
        //const auto& stops = sheet.getStops();
        //for (size_t index = 0; index < stops.size(); ++index) {
        //    if (index > 0 && stops[index].count == stops[index - 1].count) {
        //        // バックも一個扱い
        //        continue;
        //    }
        //    stopRating += BaseStopRating;
        //}

        //// BPM変化
        //constexpr double BaseBpmRating = 1.0;
        //double bpmRating = 0;
        //const auto& tempos = sheet.getTempos();
        //for (size_t index = 1; index < tempos.size(); ++index) {
        //    bpmRating += Abs(tempos[index].bpm - tempos[index - 1].bpm) * BaseBpmRating;
        //}

        //// 速度変化
        //constexpr double BaseSpeedRating = 50.0;
        //double speedRating = 0;
        //for (double diff : speedDiff) {
        //    speedRating += Min(diff - 1.0, 5.0) * BaseSpeedRating;
        //}
        return AnalyzeResult
        {
            .rating = static_cast<uint64>(Math::Round(ratingResult)),
            .meanRating = static_cast<uint64>(Math::Round(meanRating)),
            .maxRating = static_cast<uint64>(Math::Round(maxRating)),
            .medianRating = static_cast<uint64>(Math::Round(medianRating)),
        };
    }
}
