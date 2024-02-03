#include <core/Data/NotesData/NotesData.hpp>
#include <core/Data/Genre/GenreManager.hpp>
#include <core/Object/Note/Note.hpp>
#include <core/Object/Bar/Bar.hpp>
#include <core/Object/Text/TextObject.hpp>
#include <core/Object/LongNote/LongNote.hpp>
#include <core/Object/RepeatNote/RepeatNote.hpp>
#include <core/Data/Loader/ScoreLoader.hpp>
#include <queue>
#include <Siv3D.hpp>

namespace
{
    using namespace ct;

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
    NotesData::NotesData(
        const MusicData* const pMusic,
        const String& dirPath,
        const String& filePath,
        uint32 index
    ) : m_bpm(120),
        m_offsetSample(0),
        m_lv(0),
        m_lvName(U"None"),
        m_notesArtistName(U"None"),
        m_totalNotes(0),
        m_maxBarCount(0),
        m_fileName(filePath.removed(U".csv")),
        m_color(Palette::White),
        m_pMusic(pMusic),
        m_index(index)
    {
        CSV csv(dirPath + filePath);
        if (!csv)
            return;
        //セーブデータの読み込み
        m_score = ScoreLoader::Load(this->getScorePath());

        //ここから譜面データ
        this->load(csv);
    }
    void NotesData::init()
    {
        RepeatEnd::notesTapSample = 0;
        for (auto&& elm : m_objects) {
            elm->init();
        }
    }

    void NotesData::synchroCount(const Audio& sound, double& nowCount)
    {
        m_currentBarIndex = 0;
        const auto sample = GetSamplePos(sound);
        for (unsigned int i = 0; i < m_tempoInfos.size(); ++i) {
            if (sample >= m_tempoInfos.at(i).m_changeSample) {
                m_currentBarIndex = i;
            }
        }

        auto& currentTempo = m_tempoInfos.at(m_currentBarIndex).m_bar;
        const auto b = currentTempo(sound);

        nowCount = NotesData::RESOLUTION * b.bar + NotesData::RESOLUTION * (b.f);
    }

    void NotesData::update(Audio& sound, double& nowCount, [[maybe_unused]]Score& score)
    {

        this->synchroCount(sound, nowCount);

        const int64 samplePos = GetSamplePos(sound);
        if (samplePos < 3)
            return;

        PlayContext context{ samplePos, m_tempoInfos.at(m_currentBarIndex).m_bar.getBPM()};
        for (auto&& elm : m_objects) {
            if (!elm->update(context)) {
                break;
            }
        }
    }
    void NotesData::draw(double nowCount, double scrollRate)const
    {
        for (auto it = m_objects.rbegin(); it != m_objects.rend(); it++) {
            (*it)->draw(nowCount, scrollRate);
        }
    }

    void NotesData::previewDraw(double nowCount, double scrollRate)const
    {
        for (auto it = m_objects.rbegin(); it != m_objects.rend(); it++) {
            if ((*it)->getDrawCount() >= nowCount)
                (*it)->draw(nowCount, scrollRate);
        }
    }

    String NotesData::getLevelWithStar() const
    {
        if (m_starLv != StarLv::None) {
            return ToStr(m_starLv) + U":" + Format(m_lv);
        }
        return Format(m_lv);
    }

    const String NotesData::getLevelNameAndLevel() const
    {
        return m_lvName + U" Lv" + this->getLevelWithStar();
    }

    String NotesData::getScorePath()const
    {
        return U"UserData/Score/"
            + m_pMusic->getGenreName() + U"/"
            + m_pMusic->getFileName() + U"/"
            + this->m_fileName
            + U".bin";
    }

    void NotesData::load(CSV& csv)
    {
        auto rows = csv.rows();				//行数
        String head;						//1列目のデータを文字列で
        int bar = 0;						//現在入力中の小節
        std::queue<double> noteSpeed;		//ノーツのスピード変化を覚えておく
        std::queue<double> barSpeed;		//小節線のスピード変化を覚えておく
        std::queue<double> measures;		//拍子記憶用
        double nowMeasure = 1.0;				//拍子の初期化
        std::shared_ptr<Note> parentNote = std::make_shared<Note>(0, 0, 0, 0);	//ノーツの記憶(ロング用)
        uint32 totalNotes = 0;					//ノーツ数
        double scrollBaseSpeed = 1.0;
        double repeatInterval = 8.0;					//連打間隔

        BPMType nowBPM = 120;

        //譜面停止用
        Array<StopInfo> stopInfos;
        stopInfos.emplace_back();
        stopInfos.emplace_back();
        //bpm変更用
        Array<TempoInfo> tempoInfo;
        double lastBPMChangeCount = 0;
        int64 totalSample = static_cast<s3d::int64>(44100) * 4;

        struct BpmHistory
        {
            BPMType bpm;
            double changeCount;
            int64 changeSample;
        };
        Array<BpmHistory> bpmHistory;
        bpmHistory.push_back({nowBPM, lastBPMChangeCount, totalSample});

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
            const double preBPMSample = (count - changeCount) * samplePerBar / static_cast<double>(NotesData::RESOLUTION);
            return changeSample + static_cast<int64>(preBPMSample);
        };
        for (unsigned int i = 0; i < rows; i++) {
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

                for (int j = 0; j < col; ++j) //この小節のノーツ読み込み
                {
                    int type = csv.get<int>(i, j);

                    if (!type) {
                        if (!noteSpeed.empty())
                            noteSpeed.pop();
                        continue;	//0の場合スルー
                    }

                    //ノーツ数数える
                    if (type != 10)
                        totalNotes++;

                    //カウント値
                    double count = nowCount + NotesData::RESOLUTION * nowMeasure * j / col;

                    double judgeOffset = GetJudgeOffset(count, stopInfos);
                    const double fixedCount = count + judgeOffset;
                    const int64 timingSample = calcTimingSample(fixedCount);

                    //ノーツのスピード
                    double spd;
                    if (!noteSpeed.empty()) {
                        spd = noteSpeed.front();
                        noteSpeed.pop();
                    } else
                        spd = scrollBaseSpeed;

                    //ノーツ生成
                    std::shared_ptr<Note> note;

                    if (type == 10)//連打ノーツかどうか
                    {
                        note = std::make_shared<RepeatNote>(timingSample, fixedCount, spd);
                    } else {
                        note = std::make_shared<Note>(timingSample, type, fixedCount, spd);
                    }
                    if (type >= 10 && type <= 17)//ロングノーツの場合親ノーツを保存
                        parentNote = note;

                    //ベクタに追加
                    if (type == 8) {
                        if (parentNote->getType() == 10)//親が連打ノーツか
                        {
                            m_objects.emplace_back(std::make_shared<RepeatEnd>(timingSample, fixedCount, spd, parentNote, repeatInterval));
                        } else {
                            m_objects.emplace_back(std::make_shared<LongNote>(timingSample, parentNote->getType(), fixedCount, spd, parentNote));
                        }
                    } else {
                        m_objects.emplace_back(note);
                    }

                }
                measures.push(nowMeasure);
                nowCount += NotesData::RESOLUTION * nowMeasure;
                bar++;					//次の小節へ
            } else						//その他
            {
                if (head == U"#NOTE") {
                    m_notesArtistName = csv.getOr<String>(i, 1, U"None");
                } else if (head == U"#LEVEL") {
                    m_lv = csv.getOr<int>(i, 1, 0);
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
                    m_bpm = csv.getOr<BPMType>(i, 1, 120);
                    bpmHistory.back().bpm = nowBPM = m_bpm;
                } else if (head == U"#OFFSET") {
                    m_offsetSample = csv.getOr<int>(i, 1, 0);
                    totalSample += m_offsetSample;
                    bpmHistory.back().changeSample = totalSample;
                } else if (head == U"#SCROLL") {
                    size_t col = csv.columns(i);
                    for (size_t j = 1; j < col; ++j)
                        noteSpeed.push(csv.get<double>(i, j));
                } else if (head == U"#BPMCHANGE") {
                    /*
                    240:9600=x:n
                    240*n/9600
                    */
                    double count = nowCount + NotesData::RESOLUTION * nowMeasure * csv.getOr<double>(i, 2, 0) / csv.getOr<double>(i, 3, 1);
                    const double judgeOffset = GetJudgeOffset(count, stopInfos);
                    count += judgeOffset;

                    const BPMType bpm = csv.getOr<BPMType>(i, 1, 120);
                    {
                        const double tmp1 = 4 * 44100 * 60 / nowBPM;
                        const double tmp2 = 4 * 44100 * 60 / bpm;

                        double preBPMSample = (count - lastBPMChangeCount) * tmp1 / static_cast<double>(NotesData::RESOLUTION);
                        totalSample += static_cast<int64>(preBPMSample);
                        tempoInfo.emplace_back(totalSample, static_cast<int64>(totalSample - count / static_cast<double>(NotesData::RESOLUTION) * tmp2), bpm);
                    }
                    nowBPM = bpm;
                    lastBPMChangeCount = count;

                    bpmHistory.push_back({nowBPM, lastBPMChangeCount, totalSample});
                } else if (head == U"#STOP") {
                    double count = nowCount + NotesData::RESOLUTION * nowMeasure * csv.getOr<double>(i, 3, 0) / csv.getOr<double>(i, 4, 1);
                    double judgeOffset = GetJudgeOffset(count, stopInfos);
                    double range = NotesData::RESOLUTION * nowMeasure * csv.getOr<double>(i, 1, 0) / csv.getOr<double>(i, 2, 1);
                    m_stopRanges.emplace_back(count + judgeOffset, range);
                } else if (head == U"#DIRECTSTOP") {
                    double count = nowCount + NotesData::RESOLUTION * nowMeasure * csv.getOr<double>(i, 2, 0) / csv.getOr<double>(i, 3, 1);
                    const double stopSec = csv.getOr<double>(i, 1, 120);
                    const double range = stopSec * nowBPM * static_cast<double>(NotesData::RESOLUTION) / 240.0;
                    stopInfos.emplace_back(count, stopInfos[stopInfos.size() - 1].m_totalOffset + range);

                    double judgeOffset = GetJudgeOffset(count, stopInfos);
                    m_stopRanges.emplace_back(count + judgeOffset, range);
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
                    double count = nowCount + NotesData::RESOLUTION * nowMeasure * csv.getOr<double>(i, 3, 0) / csv.getOr<double>(i, 4, 1);
                    const double drawSec = csv.getOr<double>(i, 2, 2);
                    const String msg = (csv[i].size() >= 2) ? csv[i][1] : U"";

                    const double judgeOffset = GetJudgeOffset(count, stopInfos);
                    const double fixedCount = count + judgeOffset;
                    const int64 timingSample = calcTimingSample(fixedCount);

                    m_objects.emplace_back(std::make_shared<TextObject>(timingSample, fixedCount, msg, drawSec));
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

        for (int i = 0; i < bar; ++i) {
            double judgeOffset = GetJudgeOffset(nowBarCount, stopInfos);
            //		nowBarCount += judgeOffset;
            m_objects.emplace_back(std::make_shared<Bar>(nowBarCount + judgeOffset, barSpeed.front()));
            nowBarCount += NotesData::RESOLUTION * measures.front();
            measures.pop();
            barSpeed.pop();
        }

        for (auto&& note : m_objects) {
            for (auto&& s : m_stopRanges) {
                note->addStopCount(s);
            }
        }

        m_maxBarCount = nowBarCount + GetJudgeOffset(nowBarCount, stopInfos) + NotesData::RESOLUTION * 2;

        m_tempoInfos.emplace_back(0, 44100 * 4 + m_offsetSample, m_bpm);
        std::copy(tempoInfo.begin(), tempoInfo.end(), std::back_inserter(m_tempoInfos));

        //同期クラス作成等の初期化
        init();

        int lv = m_lv;
        auto starLv = m_starLv;
        if (m_starLv != StarLv::None) {
            GenreManager::Add(GenreType::StarLv, Format(U"LEVEL:", ToStr(starLv)), [starLv](const MusicData& music)->bool {
                return music.getNotesData().any([starLv](const NotesData& notes) {return notes.getStarLv() == starLv; });
            });
        } else {
            GenreManager::Add(GenreType::Lv, Format(U"LEVEL:", lv), [lv](const MusicData& music)->bool {
                return music.getNotesData().any([lv](const NotesData& notes) {return notes.getLevel() == lv; });
            }, lv);
        }
    }

    void NotesData::saveScore(const ScoreModel& score) const
    {
        ScoreLoader::Save(this->getScorePath(), score);
    }

}
