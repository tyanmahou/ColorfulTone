#include <core/Data/NotesData/SheetMusic.hpp>
#include <Siv3D.hpp>

namespace
{
    using namespace ct;

    struct StopInfo
    {
        double m_firstCount;  //最終更新のあったストップのカウント
        double m_totalOffset; //今までのトータルの停止時間におるオフセットぶん
        StopInfo() :
            m_firstCount(0),
            m_totalOffset(0)
        {}
        StopInfo(double firstCount, double totalOffset) :
            m_firstCount(firstCount),
            m_totalOffset(totalOffset)
        {}
    };
    struct BpmHistory
    {
        BPMType bpm;
        double changeCount;
        int64 changeSample;
    };

    size_t GetLastStopIndex(const double count, const Array<StopInfo>& stopInfos)
    {
        for (size_t k = 0; k < stopInfos.size(); ++k) {
            if (count < stopInfos[k].m_firstCount) {
                return k - 1;
            }
        }
        return stopInfos.size() - 1;
    }

    double GetJudgeOffset(const double count, const Array<StopInfo>& stopInfos)
    {
        double judgeOffset = 0;

        size_t lastStopIndex = GetLastStopIndex(count, stopInfos);

        if (count > stopInfos[lastStopIndex].m_firstCount)
            judgeOffset = stopInfos[lastStopIndex].m_totalOffset;
        else
            judgeOffset = stopInfos[lastStopIndex - 1].m_totalOffset;

        return judgeOffset;
    }
}
namespace ct
{
    SheetMusic::SheetMusic():
        m_offsetSample(0),
        m_lv(0),
        m_lvName(U"None"),
        m_notesArtistName(U"None"),
        m_totalNotes(0),
        m_lastBarCount(0),
        m_color(Palette::White)
    {
    }
    SheetMusic::SheetMusic(s3d::FilePathView path) :
        SheetMusic()
    {
        this->load(path);
    }
    bool SheetMusic::load(s3d::FilePathView path)
    {
        CSV csv(path);
        if (!csv) {
            return false;
        }

        size_t rows = csv.rows();			// 行数
        String head;						// 1列目のデータを文字列で
        int32 bar = 0;						// 現在入力中の小節
        std::queue<double> noteSpeed;		// ノーツのスピード変化を覚えておく
        std::queue<double> barSpeed;		// 小節線のスピード変化を覚えておく
        std::queue<double> measures;		// 拍子記憶用
        double nowMeasure = 1.0;			// 拍子の初期化
        uint32 totalNotes = 0;				// ノーツ数
        double scrollBaseSpeed = 1.0;
        double repeatInterval = 8.0;		// 連打間隔

        BPMType nowBPM = 120;

        //譜面停止用
        Array<::StopInfo> stopInfos{ {},{} };
        //bpm変更用
        double lastBPMChangeCount = 0;
        int64 totalSample = static_cast<s3d::int64>(44100) * 4;
        Array<::BpmHistory> bpmHistory;
        bpmHistory.push_back({ nowBPM, lastBPMChangeCount, totalSample });
        m_tempos.push_back(TempoEntity{
            .sample = 0,
            .count = 0,
            .bpm = nowBPM,
            .bpmOffsetSample = totalSample
            });
        double nowCount = 0;
        const auto calcTimingSample = [&](double count)->int64 {
            size_t historyIndex = 0;
            for (size_t i = bpmHistory.size() - 1; i >= 0; --i) {
                if (count >= bpmHistory[i].changeCount) {
                    historyIndex = i;
                    break;
                }
            }
            const BPMType bpm = bpmHistory[historyIndex].bpm;
            const double changeCount = bpmHistory[historyIndex].changeCount;
            const int64 changeSample = bpmHistory[historyIndex].changeSample;

            const double samplePerBar = 4 * 44100 * 60 / bpm;
            const double preBPMSample = (count - changeCount) * samplePerBar / static_cast<double>(SheetMusic::RESOLUTION);
            return changeSample + static_cast<int64>(preBPMSample);
            };
        for (size_t i = 0; i < rows; i++) {
            head = csv.get<String>(i, 0);

            if (head.isEmpty())			//空行はスルー
            {
                continue;
            }
            if (head[0] == U'%')//コメント
            {
                continue;
            }
            if (head[0] != U'#')			//ノーツ
            {
                size_t col = csv.columns(i);

                //小節線のスピード保存
                double bs = noteSpeed.empty() ? scrollBaseSpeed : noteSpeed.front();
                barSpeed.push(bs);

                for (size_t j = 0; j < col; ++j) //この小節のノーツ読み込み
                {
                    int32 type = csv.get<int32>(i, j);

                    if (!type) {
                        if (!noteSpeed.empty())
                            noteSpeed.pop();
                        continue;	//0の場合スルー
                    }

                    //ノーツ数数える
                    if (type != 10)
                        totalNotes++;

                    //カウント値
                    double count = nowCount + RESOLUTION * nowMeasure * j / col;

                    double judgeOffset = GetJudgeOffset(count, stopInfos);
                    const double fixedCount = count + judgeOffset;
                    const int64 timingSample = calcTimingSample(fixedCount);

                    //ノーツのスピード
                    double spd;
                    if (!noteSpeed.empty()) {
                        spd = noteSpeed.front();
                        noteSpeed.pop();
                    } else {
                        spd = scrollBaseSpeed;
                    }

                    //ノーツ生成
                    m_notes.push_back(NoteEntity{
                        .sample = timingSample,
                        .count = fixedCount,
                        .type = type,
                        .speed = spd,
                        .interval = repeatInterval
                    });
                }
                measures.push(nowMeasure);
                nowCount += RESOLUTION * nowMeasure;
                bar++; //次の小節へ
            }
            else						
            {
                //その他
                if (head == U"#NOTE") {
                    m_notesArtistName = csv.getOr<String>(i, 1, U"None");
                } else if (head == U"#LEVEL") {
                    m_lv = csv.getOr<int32>(i, 1, 0);
                    m_lvName = csv.getOr<String>(i, 2, U"None");

                    auto starKind = csv.getOpt<String>(i, 3);
                    if (starKind) {
                        if (*starKind == U"★") {
                            m_starLv = StarLv::One;
                        } else if (*starKind == U"★★") {
                            m_starLv = StarLv::Two;
                        }
                    } else {
                        if (m_lv == 14) {
                            m_starLv = StarLv::One;
                        } else if (m_lv >= 15) {
                            m_starLv = StarLv::Two;
                        }
                    }
                } else if (head == U"#BPM") {
                    const BPMType nextBPM = csv.getOr<BPMType>(i, 1, 120);
                    m_tempos.back().bpm = bpmHistory.back().bpm = nextBPM;
                } else if (head == U"#OFFSET") {
                    m_offsetSample = csv.getOr<int64>(i, 1, 0);
                    totalSample += m_offsetSample;
                    nowBPM = m_tempos.back().bpmOffsetSample = bpmHistory.back().changeSample = totalSample;
                } else if (head == U"#SCROLL") {
                    size_t col = csv.columns(i);
                    for (size_t j = 1; j < col; ++j) {
                        noteSpeed.push(csv.get<double>(i, j));
                    }
                } else if (head == U"#BPMCHANGE") {
                    /*
                    240:9600=x:n
                    240*n/9600
                    */
                    double count = nowCount + RESOLUTION * nowMeasure * csv.getOr<double>(i, 2, 0) / csv.getOr<double>(i, 3, 1);
                    const double judgeOffset = GetJudgeOffset(count, stopInfos);
                    count += judgeOffset;

                    const BPMType bpm = csv.getOr<BPMType>(i, 1, 120);
                    {
                        const double tmp1 = 4 * 44100 * 60 / nowBPM;
                        const double tmp2 = 4 * 44100 * 60 / bpm;

                        double preBPMSample = (count - lastBPMChangeCount) * tmp1 / static_cast<double>(RESOLUTION);
                        totalSample += static_cast<int64>(preBPMSample);
                        const int64 offs = static_cast<int64>(totalSample - count / static_cast<double>(RESOLUTION) * tmp2);
                        m_tempos.push_back(TempoEntity{
                            .sample = totalSample,
                            .count = count,
                            .bpm = bpm,
                            .bpmOffsetSample = offs
                            });
                    }
                    nowBPM = bpm;
                    lastBPMChangeCount = count;

                    bpmHistory.push_back({ nowBPM, lastBPMChangeCount, totalSample });
                } else if (head == U"#STOP") {
                    double count = nowCount + RESOLUTION * nowMeasure * csv.getOr<double>(i, 3, 0) / csv.getOr<double>(i, 4, 1);
                    double judgeOffset = GetJudgeOffset(count, stopInfos);
                    double range = RESOLUTION * nowMeasure * csv.getOr<double>(i, 1, 0) / csv.getOr<double>(i, 2, 1);
                    m_stops.push_back(StopEntity{
                        .count = count + judgeOffset,
                        .rangeCount = range
                        });
                } else if (head == U"#DIRECTSTOP") {
                    double count = nowCount + RESOLUTION * nowMeasure * csv.getOr<double>(i, 2, 0) / csv.getOr<double>(i, 3, 1);
                    const double stopSec = csv.getOr<double>(i, 1, 120);
                    const double range = stopSec * nowBPM * static_cast<double>(RESOLUTION) / 240.0;
                    stopInfos.emplace_back(count, stopInfos[stopInfos.size() - 1].m_totalOffset + range);

                    double judgeOffset = GetJudgeOffset(count, stopInfos);
                    m_stops.push_back(StopEntity{
                        .count = count + judgeOffset,
                        .rangeCount = range
                        });
                } else if (head == U"#MEASURE") {
                    nowMeasure = csv.getOr<double>(i, 1, 1.0) / csv.getOr<double>(i, 2, 1.0);
                } else if (head == U"#SCROLLBASE") {
                    scrollBaseSpeed = csv.getOr<double>(i, 1, 1.0);
                } else if (head == U"#INTERVAL") {
                    repeatInterval = csv.getOr<double>(i, 1, 8.0);
                    if (repeatInterval <= 0) {
                        repeatInterval = 8.0;
                    }
                } else if (head == U"#TEXT") {
                    double count = nowCount + RESOLUTION * nowMeasure * csv.getOr<double>(i, 3, 0) / csv.getOr<double>(i, 4, 1);
                    const double drawSec = csv.getOr<double>(i, 2, 2);
                    const String msg = (csv[i].size() >= 2) ? csv[i][1] : U"";

                    const double judgeOffset = GetJudgeOffset(count, stopInfos);
                    const double fixedCount = count + judgeOffset;
                    const int64 timingSample = calcTimingSample(fixedCount);

                    m_lyrics.push_back(LyricEntity{
                        .sample = timingSample,
                        .count= fixedCount,
                        .message = msg,
                        .timeSec = drawSec
                        });
                } else if (head == U"#COLOR") {
                    const String colorHex = csv.getOr<String>(i, 1, U"#FFFFFF");
                    m_color = Color(colorHex);
                } else if (head == U"#END") {
                    break;
                }
            }
        }

        m_totalNotes = totalNotes;

        //小節線作成
        double nowBarCount = 0;

        for (int32 i = 0; i < bar; ++i) {
            double judgeOffset = GetJudgeOffset(nowBarCount, stopInfos);
            double fixedCount = nowBarCount + judgeOffset;
            m_bars.push_back(BarEntity{
                .count = fixedCount,
                .speed = barSpeed.front(),
                });
            nowBarCount += RESOLUTION * measures.front();
            measures.pop();
            barSpeed.pop();
        }

        m_lastBarCount = nowBarCount + GetJudgeOffset(nowBarCount, stopInfos) + RESOLUTION * 2;

        return true;
    }
    s3d::String ct::SheetMusic::getLevelWithStar() const
    {
        if (m_starLv != StarLv::None) {
            return ToStr(m_starLv) + U":" + Format(m_lv);
        }
        return Format(m_lv);
    }
    s3d::String SheetMusic::getLevelNameAndLevel() const
    {
        return m_lvName + U" Lv" + this->getLevelWithStar();
    }
}
