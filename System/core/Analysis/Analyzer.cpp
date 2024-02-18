﻿#include <core/Analysis/Analyzer.hpp>
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
            return 1.2 * note.interval / 8.0;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
            return 1.2;
        case 18:
            return 0.8;
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
        constexpr int64 JackThresholdMin = 0;
        constexpr int64 JackThresholdMax = 22050; // bpm120での1拍
        auto calcJackFactor = [](int64 diff) {
            int64 clampDiff = Clamp<int64>(diff, JackThresholdMin, JackThresholdMax);
            double r = Math::InvLerp(JackThresholdMin, JackThresholdMax, static_cast<double>(clampDiff));
            return 1.0 * s3d::Pow(-(r - 1), LogBase(0.25, 0.75)) + 1;
        };
        auto supJackFactor = [&](int64 diff) {
            return (3.0 * calcJackFactor(diff) + 2.0) / 5.0;
            };
        auto logJackFactor = [&](int64 diff) {
            return Math::Log(calcJackFactor(diff)) + 1.0;
         };
        // ノーツ1つにつき
        constexpr double BaseNoteRating = 1000.0;
        // ロング終点以外
        const auto notes = sheet.getNotes().filter([](const NoteEntity& e) {
            return e.type != 8;
        });

        Array<std::pair<int64, double>> notesRatings;
        notesRatings.reserve(notes.size());
        Array<double> speedDiff;
        {
            int64 lastSample = 0;
            std::array<int64, 3> lastSampleBits{ 0,0,0 };

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

        // BPM変化 1につき
        constexpr double BaseBpmRating = 100.0;
        constexpr double BpmDiffThresholdMax = 150.0;
        Array<std::pair<int64, double>> bpmRatings;
        {
            const auto& tempos = sheet.getTempos();
            for (size_t index = 1; index < tempos.size(); ++index) {
                BPMType bpmDiff = Min<BPMType>(Abs(tempos[index].bpm - tempos[index - 1].bpm), BpmDiffThresholdMax);

                double rating = BaseBpmRating * logJackFactor(tempos[index].sample - tempos[index - 1].sample) * bpmDiff;
                bpmRatings.emplace_back(tempos[index].sample, rating);
            }
        }
                
        // 2秒毎にレーティング 計算
        Array<double> barRatings;
        {
            int64 startSample = sheet.getTempos()[0].bpmOffsetSample;
            int64 endSample = sheet.getOffsetedTotalSample();
            size_t notesIndex = 0;
            size_t bpmIndex = 0;
            for (int64 nextSample = startSample; nextSample <= endSample; nextSample += (44100 * 2)) {
                double noteSum = 0;
                // ノーツレート
                while (notesIndex < notesRatings.size() && notesRatings[notesIndex].first < nextSample) {
                    noteSum += notesRatings[notesIndex].second;
                    ++notesIndex;
                }
                if (noteSum <= 0) {
                    // 何もないなら含めない
                    continue;
                }
                // BPM変化レート
                double bpmSum = 0;
                //while (bpmIndex < bpmRatings.size() && bpmRatings[bpmIndex].first < nextSample) {
                //    bpmSum += bpmRatings[bpmIndex].second;
                //    ++bpmIndex;
                //}
                double barRate = noteSum + bpmSum;
                barRatings.push_back(barRate);
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
