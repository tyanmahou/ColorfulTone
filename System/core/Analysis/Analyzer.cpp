#include <core/Analysis/Analyzer.hpp>
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

        Array<double> notesRating;
        Array<double> speedDiff;
        int64 lastSample = 0;
        std::array<int64, 3> lastSampleBits{ 0,0,0 };

        notesRating.reserve(notes.size());
        //if (!notes.isEmpty()) {
        //    notesRating.push_back(BaseNoteRating / 4.0 * TypeFactor(notes[0]));
        //}
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
            if (diff == 0) {
                // 無いはずだけどDiffがないなら
                notesRating.push_back(BaseNoteRating * TypeFactor(notes[index]));
            } else {
                double score = BaseNoteRating / (static_cast<double>(diff) / 44100.0) * TypeFactor(notes[index]);
                notesRating.push_back(score);
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

        // 平均レーティング
        double aveRating = 0;
        if (!notesRating.isEmpty()) {
            double sum = 0;
            for (double r : notesRating) {
                sum += r;
            }
            aveRating = sum / notesRating.size();
        }
        // 局所レーティング
        double localAveRating = 0;
        if (notesRating.size() <= 0) {
            localAveRating = aveRating;
        } else if (notesRating.size() <= 16) {
            Array<double> midMemo = notesRating;
            midMemo.sort();
            // 中央値と平均値のミックス
            size_t half = notesRating.size() / 2;
            const double mid = notesRating.size() % 2 == 1 ?
                midMemo[half] :
                (midMemo[half] + midMemo[half - 1]) / 2.0
                ;
            localAveRating = aveRating * 0.1 + mid * 0.9;
        } else {
            for (size_t index = 0; index < notesRating.size() - 16; ++index) {
                double sum = 0;
                Array<double> midMemo;
                for (size_t index2 = 0; index2 < 16; ++index2) {
                    sum += notesRating[index + index2];
                    midMemo.push_back(notesRating[index + index2]);
                }
                midMemo.sort();
                // 中央値と平均値のミックス
                const double ave = sum / 16.0;
                const double mid = (midMemo[8] + midMemo[7]) / 2.0;
                if (double localAve = ave * 0.1 + mid * 0.9; localAve > localAveRating) {
                    localAveRating = localAve;
                }
            }
        }
        // ノーツ レート
        const double notesRatingResult = localAveRating * 0.7 + aveRating * 0.3;

        // 停止レート 1個につき
        constexpr double BaseStopRating = 100.0;
        double stopRating = 0;
        //const auto& stops = sheet.getStops();
        //for (size_t index = 0; index < stops.size(); ++index) {
        //    if (index > 0 && stops[index].count == stops[index - 1].count) {
        //        // バックも一個扱い
        //        continue;
        //    }
        //    stopRating += BaseStopRating;
        //}
        // BPM変化
        constexpr double BaseBpmRating = 1.0;
        double bpmRating = 0;
        //const auto& tempos = sheet.getTempos();
        //for (size_t index = 1; index < tempos.size(); ++index) {
        //    bpmRating += Abs(tempos[index].bpm - tempos[index - 1].bpm) * BaseBpmRating;
        //}
        // 速度変化
        constexpr double BaseSpeedRating = 50.0;
        double speedRating = 0;
        //for (double diff : speedDiff) {
        //    speedRating += Min(diff - 1.0, 5.0) * BaseSpeedRating;
        //}

        const double ratingResult = notesRatingResult + stopRating + bpmRating + speedRating;
        return AnalyzeResult
        {
            .rating = static_cast<uint64>(Math::Round(ratingResult)),
            .aveRating = static_cast<uint64>(Math::Round(aveRating)),
            .localAveRating = static_cast<uint64>(Math::Round(localAveRating)),
            .stopRating = static_cast<uint64>(Math::Round(stopRating)),
            .bpmRating = static_cast<uint64>(Math::Round(bpmRating)),
            .speedRating = static_cast<uint64>(Math::Round(speedRating)),
        };
    }
}
