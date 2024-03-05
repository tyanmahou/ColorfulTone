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
        case 4:
        case 5:
        case 6:
        case 7:
            return 1.0;
        case 8:
            return 0.0;
        case 9:
            return 0.1;
        case 10:
            return 1.0 * note.interval / 8.0;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
            return 1.2;
        case 18:
            return 0.7;
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
    double LogBase(double a, double b)
    {
        return s3d::Math::Log(a) / s3d::Math::Log(b);
    }
}
namespace ct
{

    AnalyzeResult Analyzer::Analyze(const SheetMusic& sheet)
    {
        constexpr double BackFactor = 1.5;
        // ノーツ1つにつき
        constexpr double BaseNoteRating = 1000.0;

        // 縦連補正
        constexpr int64 JackThresholdMin = 0;
        constexpr int64 JackThresholdMax = 22050; // bpm120での1拍
        auto calcJackFactor = [](int64 diff) {
            int64 clampDiff = Clamp<int64>(diff, JackThresholdMin, JackThresholdMax);
            double r = Math::InvLerp(JackThresholdMin, JackThresholdMax, static_cast<double>(clampDiff));
            return 1.5 * s3d::Pow(-(r - 1), LogBase(0.5, (1 - 0.125))) + 1;
            };

        // 速度補正
        constexpr double SpeedRatioMax = 3.0;
        auto calcSpeedRatingFactor = [](double ratio) {
            ratio = Min(ratio, SpeedRatioMax);
            return 1.0 + 0.6 * EaseInOutSine(Math::InvLerp(1, SpeedRatioMax, ratio));
            };
        auto calcSpeedDiffRatingFactor = [](double ratio) {
            ratio = Min(ratio, SpeedRatioMax);
            return Math::InvLerp(1, SpeedRatioMax, ratio);
            };
        // ロング終点以外
        const auto notes = sheet.getNotes().filter([](const NoteEntity& e) {
            return e.type != 8;
            });

        const auto& tempos = sheet.getTempos();

        // 速度基準の計算
        Array<double> speeds;
        speeds.reserve(notes.size());
        {
            size_t notesIndex = 0;

            for (size_t tempoIndex = 0; tempoIndex < tempos.size(); ++tempoIndex) {
                int64 nextSample = tempoIndex + 1 < tempos.size() ? tempos[tempoIndex + 1].sample : sheet.getOffsetedTotalSample();

                while (notesIndex < notes.size() && notes[notesIndex].sample < nextSample) {
                    // BPM考慮した
                    speeds.push_back(Abs(tempos[tempoIndex].bpm * notes[notesIndex].speed));
                    ++notesIndex;
                }
            }
        }
        // 基準速度
        double speedBase = StatisticsUtil::GeometricMeanInIQRBounds(speeds);
        Array<std::pair<int64, double>> notesRatings;
        notesRatings.reserve(notes.size());
        {
            int64 lastSample = 0;
            std::array<int64, 3> lastSampleBits{ 0,0,0 };

            for (size_t index = 0; index < notes.size(); ++index) {
                int32 typebit = NoteTypeBit(notes[index].type);
                Array<int64> nearSample{};
                if (typebit == 0) {
                    nearSample << lastSample;
                } else {
                    if ((typebit & 0b001) != 0) { // 赤
                        nearSample << lastSampleBits[0];
                    }
                    if ((typebit & 0b010) != 0) { // 青
                        nearSample << lastSampleBits[1];
                    }
                    if ((typebit & 0b100) != 0) { // 黄
                        nearSample << lastSampleBits[2];
                    }
                }
                double rating = 0;
                for (auto near : nearSample) {
                    rating += BaseNoteRating * calcJackFactor(notes[index].sample - near) * TypeFactor(notes[index]);
                }

                if (speeds[index] >= 10000) {
                    // ありえんほどデカい場合は見えないノーツなので別扱い
                    rating += 3000;
                } else {

                    // 速度倍率補正
                    {
                        double lowSpeed = Min(speeds[index], speedBase);
                        double highSpeed = Max(speeds[index], speedBase);
                        double speedRatio = 0;
                        if (lowSpeed == 0) {
                            speedRatio = 2;
                        } else {
                            speedRatio = highSpeed / lowSpeed;
                        }

                        rating *= calcSpeedRatingFactor(speedRatio);

                        // 逆走むずい
                        double backFactor = (notes[index].speed < 0) ? BackFactor : 1.0;
                        rating *= backFactor;
                    }
                }
                if (index > 0 && (notes[index].type == 9 && notes[index - 1].type == 9) && (index + 1 >= notes.size() || notes[index + 1].type == 9)) {
                    // 連続する白ノーツは特殊的に弱くする
                    rating /= 10.0;
                }

                notesRatings.emplace_back(notes[index].sample, rating);

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
        }

        // BPM変化 1につき
        constexpr double BaseBpmRating = 25.0;
        constexpr double BpmRatingFactorMax = 200.0;
        Array<std::pair<int64, double>> bpmRatings;
        bpmRatings.reserve(sheet.getTempos().size());
        {
            size_t notesIndex = 0;
            for (size_t index = 1; index < tempos.size(); ++index) {
                // 影響範囲開始のノーツまで進める
                for (; notesIndex < notes.size(); ++notesIndex) {
                    if (notes[notesIndex].sample > tempos[index].sample) {
                        break;
                    }
                }
                if (notesIndex == notes.size()) {
                    // この停止より後ろにノーツはない
                    break;
                }
                BPMType bpmDiff = Abs(tempos[index].bpm - tempos[index - 1].bpm);
                if (bpmDiff <= 0) {
                    continue;
                }
                if (notes[notesIndex].sample > tempos[index].sample + (44100 * 2)) {
                    // 2秒より離れているならレート換算ほぼなし
                    bpmRatings.emplace_back(tempos[index].sample, Min<BPMType>(bpmDiff, BpmRatingFactorMax));
                    continue;
                }
                double bpmFactor = Min<BPMType>(100.0 * Pow(bpmDiff / 100.0, LogBase(0.75, 0.6)), BpmRatingFactorMax);
                double rating = BaseBpmRating * bpmFactor;
                bpmRatings.emplace_back(tempos[index].sample, rating);
            }
        }

        // 停止レート 1個につき
        constexpr double BaseStopRating = 1000.0;
        constexpr double OverlapStopRating = 200.0;
        Array<std::pair<int64, double>> stopRatings;
        stopRatings.reserve(sheet.getStops().size());
        {
            const auto& stops = sheet.getStops();

            struct StopTemp
            {
                int64 beginSample;
                int64 endSample;
                bool isBack = false;
            };
            Array<StopTemp> stopTemps;
            for (size_t index = 0; index < stops.size(); ++index) {
                int64 endSample = sheet.calcTimingSample(stops[index].count + stops[index].rangeCount);
                if (index > 0 && stops[index].count == stops[index - 1].count) {
                    // バックも一個扱い
                    if (endSample > stopTemps.back().endSample) {
                        // 影響範囲更新
                        stopTemps.back().endSample = endSample;
                    }
                    stopTemps.back().isBack = true;
                    continue;
                }
                stopTemps.emplace_back(stops[index].sample, endSample, false);
            }
            size_t notesIndex = 0;
            for (size_t index = 0; index < stopTemps.size(); ++index) {
                auto [beginSample, endSample, isBack] = stopTemps[index];

                // 影響範囲開始のノーツまで進める
                for (; notesIndex < notes.size(); ++notesIndex) {
                    if (notes[notesIndex].sample > beginSample) {
                        break;
                    }
                }
                if (notesIndex == notes.size()) {
                    // この停止より後ろにノーツはない
                    break;
                }
                if (notes[notesIndex].sample > endSample + (44100 * 2)) {
                    // 2秒より離れているならレート換算ほぼなし
                    stopRatings.emplace_back(beginSample, 10);
                    continue;
                }

                // 停止に被るノーツは難しい
                int64 midSample = (beginSample + endSample) / 2;
                for (size_t i = notesIndex; i < notes.size(); ++i) {
                    if (notes[i].sample > endSample) {
                        break;
                    }
                    double overlapFactor = 1.0;
                    if (isBack) {
                        if (notes[i].sample >= midSample) {
                            overlapFactor = BackFactor;
                        }
                    }
                    notesRatings[i].second *= overlapFactor;
                    notesRatings[i].second += OverlapStopRating;
                }
                // 定数
                double rating = BaseStopRating;
                stopRatings.emplace_back(beginSample, rating);
            }
        }

        // 2秒毎にレーティング 計算
        Array<double> barRatings;
        if (!notesRatings.isEmpty()) {
            int64 startSample = sheet.getTempos()[0].bpmOffsetSample;
            int64 endSample = sheet.getOffsetedTotalSample() + (44100 * 2);
            size_t notesIndex = 0;
            size_t bpmIndex = 0;
            size_t stopIndex = 0;
            for (int64 nextSample = startSample; nextSample <= endSample; nextSample += (44100 * 2)) {
                double noteSum = 0;
                // ノーツレート
                while (notesIndex < notesRatings.size() && notesRatings[notesIndex].first < nextSample) {
                    noteSum += notesRatings[notesIndex].second;
                    ++notesIndex;
                }

                // BPM変化レート
                double bpmSum = 0;
                while (bpmIndex < bpmRatings.size() && bpmRatings[bpmIndex].first < nextSample) {
                    bpmSum += bpmRatings[bpmIndex].second;
                    ++bpmIndex;
                }
                // 譜面停止レート
                double stopSum = 0;
                while (stopIndex < stopRatings.size() && stopRatings[stopIndex].first < nextSample) {
                    stopSum += stopRatings[stopIndex].second;
                    ++stopIndex;
                }
                double barRate = noteSum + bpmSum + stopSum;
                if (barRate > 0) {
                    barRatings.push_back(barRate);
                }
            }
        }
        // 平均レート
        double meanRating = StatisticsUtil::Mean(barRatings);
        // 最大局所レート
        double maxRating = StatisticsUtil::Max(barRatings);
        // 80パーセンタイル (クリア)
        double percentile80Rating = StatisticsUtil::Percentile(barRatings, 80.0);
        // 97パーセンタイル (AAA)
        double percentile97Rating = StatisticsUtil::Percentile(barRatings, 97.0);
        // 中央局所レート (AP)
        double medianRating = StatisticsUtil::Median(barRatings);

        // レート
        const double ratingMix = meanRating * 0.2
            + medianRating * 0.17
            + percentile80Rating * 0.22
            + percentile97Rating * 0.33
            + maxRating * 0.08;

        // バラつき補正
        double speedRating = 0;
        {
            Array<double> targetSpeed;
            for (size_t index = 1; index < notes.size(); ++index) {
                if (speeds[index] >= 10000 || speeds[index] <= 0) {
                    continue;
                }
                if (notes[index].type == 9) {
                    // 白は無視
                    continue;
                }
                if (notes[index].sample < notes[index - 1].sample + (44100 * 2)) {
                    targetSpeed.push_back(notes[index].speed);
                }
            }
            Array<double> speedDiff;
            for (size_t index = 1; index < targetSpeed.size(); ++index) {
                double ratio = targetSpeed[index] / targetSpeed[index - 1];
                speedDiff << Sign(ratio) * Clamp(Abs(ratio), 1 / 5.0, 5.0);
            }
            double speedDev = StatisticsUtil::GeometricAbsDev(speedDiff);
            double rate = 1.75 * Saturate(s3d::Log(speedDev) / s3d::Log(1.5)); // Pow(1 - Pow(1 - Saturate(s3d::Log(speedDev) / s3d::Log(5.0)), 10.0), 20.0);
            speedRating = (ratingMix * Math::Lerp(1, 1.75, Math::InvLerp(0, 1.75, rate))) - ratingMix;
        }
        const double otherRating = ratingMix + speedRating;

        // ノーツ数重み補正
        const double ratingPerNote = otherRating / static_cast<double>(Max<size_t>(sheet.getTotalNotes(), 1));
        const double noteWeight = ratingPerNote * ratingPerNote / 2.0;

        const double ratingResult = otherRating + noteWeight;
        return AnalyzeResult
        {
            .rating = static_cast<uint64>(Math::Round(ratingResult)),
            .meanRating = static_cast<uint64>(Math::Round(meanRating)),
            .medianRating = static_cast<uint64>(Math::Round(medianRating)),
            .percentile80Rating = static_cast<uint64>(Math::Round(percentile80Rating)),
            .percentile97Rating = static_cast<uint64>(Math::Round(percentile97Rating)),
            .maxRating = static_cast<uint64>(Math::Round(maxRating)),
            .noteWeightRating = static_cast<uint64>(Math::Round(noteWeight)),
        };
    }
}
