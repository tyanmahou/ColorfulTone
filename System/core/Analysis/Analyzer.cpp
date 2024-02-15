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
            return 0.2;
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
}
namespace ct
{

    AnalyzeResult Analyzer::Analyze(const SheetMusic& sheet)
    {
        // 1秒間隔のスコア 反比例
        constexpr double BaseNoteRating = 1000.0;
        constexpr int64 NoteDiffThreshold = 44100 * 2;
        const auto& notes = sheet.getNotes();

        Array<double> notesRating;
        notesRating.reserve(notes.size());
        if (!notes.isEmpty()) {
            notesRating.push_back(BaseNoteRating / 4.0 * TypeFactor(notes[0]));
        }
        for (size_t index = 1; index < notes.size(); ++index) {
            int64 diff = Min(notes[index].sample - notes[index - 1].sample, NoteDiffThreshold);
            if (diff == 0) {
                // 無いはずだけどDiffがないなら
                notesRating.push_back(BaseNoteRating * TypeFactor(notes[index]));
                continue;
            }
            double score = BaseNoteRating / (static_cast<double>(diff) / 44100.0) * TypeFactor(notes[index]);
            notesRating.push_back(score);
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
        if (notesRating.size() <= 16) {
            localAveRating = aveRating;
        } else {
            for (size_t index = 0; index < notesRating.size() - 16; ++index) {
                double sum = 0;
                for (size_t index2 = 0; index2 < 16; ++index2) {
                    sum += notesRating[index + index2];
                }
                if (double ave = sum / 16.0; ave > localAveRating) {
                    localAveRating = ave;
                }
            }
        }
        // ノーツ レート
        const double notesRatingResult = localAveRating * 0.7 + aveRating * 0.3;

        const double ratingResult = notesRatingResult;
        return AnalyzeResult
        {
            .rating = static_cast<uint64>(Math::Round(ratingResult)),
            .aveRating = static_cast<uint64>(Math::Round(aveRating)),
            .localAveRating = static_cast<uint64>(Math::Round(localAveRating)),
            .stopRating = 0,
            .bpmRating = 0,
            .speedRating = 0
        };
    }
}
