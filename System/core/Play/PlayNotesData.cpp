#include <core/Play/PlayNotesData.hpp>
#include <core/Data/NotesData/NotesData.hpp>
#include <core/Data/NotesData/SheetMusic.hpp>

#include <core/Object/Note/Note.hpp>
#include <core/Object/Bar/Bar.hpp>
#include <core/Object/Text/TextObject.hpp>
#include <core/Object/LongNote/LongNote.hpp>
#include <core/Object/RepeatNote/RepeatNote.hpp>

#include <Siv3D.hpp>

namespace ct
{
    PlayNotesData::PlayNotesData()
    {

    }
    PlayNotesData::PlayNotesData(const NotesData& notesData):
        m_notesData(notesData)
    {
        this->init(m_notesData.getSheet());
    }


    void PlayNotesData::init(const SheetMusic& sheet)
    {
        //ノーツの記憶(ロング用)
        std::shared_ptr<Note> parentNote = std::make_shared<Note>(0, 0, 0, 0);

        m_objects.reserve(sheet.getNotes().size() + sheet.getLyrics().size() + sheet.getBars().size());
        // 音符追加
        for (const NoteEntity& n : sheet.getNotes()) {
            const auto& [sample, count, type, speed, interval] = n;

            //ノーツ生成
            std::shared_ptr<Note> note;
            if (type == 10)//連打ノーツかどうか
            {
                note = std::make_shared<RepeatNote>(sample, count, speed);
            } else {
                note = std::make_shared<Note>(sample, type, count, speed);
            }
            if (type >= 10 && type <= 17) {
                //ロングノーツの場合親ノーツを保存
                parentNote = note;
            }

            //ベクタに追加
            if (type == 8) {
                if (parentNote->getType() == 10)//親が連打ノーツか
                {
                    m_objects.emplace_back(std::make_shared<RepeatEnd>(sample, count, speed, parentNote, interval));
                } else {
                    m_objects.emplace_back(std::make_shared<LongNote>(sample, parentNote->getType(), count, speed, parentNote));
                }
            } else {
                m_objects.emplace_back(note);
            }
        }
        // テキスト追加
        for (const LyricEntity& l : sheet.getLyrics()) {
            const auto& [sample, count, message, timeSec] = l;
            m_objects.emplace_back(std::make_shared<TextObject>(sample, count, message, timeSec));
        }
        // 小節線追加
        for (const BarEntity& b : sheet.getBars()) {
            const auto& [_, count, speed] = b;
            m_objects.emplace_back(std::make_shared<Bar>(count, speed));
        }
        // 停止追加
        for (const StopEntity& b : sheet.getStops()) {
            m_stopRanges.emplace_back(b.count, b.rangeCount);
        }
        // テンポ情報追加
        for (const TempoEntity& t : sheet.getTempos()) {
            m_tempoInfos.emplace_back(t.sample, t.bpmOffsetSample, t.bpm);
        }

        // 譜面停止時間差分の適用
        for (auto&& note : m_objects) {
            for (auto&& s : m_stopRanges) {
                note->addStopCount(s);
            }
        }
        this->reset();
    }
    void PlayNotesData::reset()
    {
        RepeatEnd::notesTapSample = 0;
        for (auto&& elm : m_objects) {
            elm->init();
        }
    }
    void PlayNotesData::synchroCount(const s3d::Audio& sound, double& nowCount)
    {
        m_currentBarIndex = 0;
        const auto sample = GetSamplePos(sound);
        for (size_t i = 0; i < m_tempoInfos.size(); ++i) {
            if (sample >= m_tempoInfos.at(i).m_changeSample) {
                m_currentBarIndex = i;
            }
        }

        const SoundBar& currentTempo = m_tempoInfos.at(m_currentBarIndex).m_bar;
        const auto b = currentTempo(sound);

        nowCount = NotesData::RESOLUTION * b.bar + NotesData::RESOLUTION * (b.f);
    }
    void PlayNotesData::update(const s3d::Audio& sound, double& nowCount, [[maybe_unused]]Score& score)
    {
        this->synchroCount(sound, nowCount);

        const int64 samplePos = GetSamplePos(sound);
        if (samplePos < 3)
            return;
        const int64 ajudstSample = 735 * Game::Config().m_timingAdjust;
        PlayContext context{ samplePos + ajudstSample, m_tempoInfos.at(m_currentBarIndex).m_bar.getBPM() };
        for (auto&& elm : m_objects) {
            if (!elm->update(context)) {
                break;
            }
        }
    }
    void PlayNotesData::draw(double nowCount, double scrollRate) const
    {
        for (auto it = m_objects.rbegin(); it != m_objects.rend(); it++) {
            (*it)->draw(nowCount, scrollRate);
        }
    }
    void PlayNotesData::previewDraw(double nowCount, double scrollRate) const
    {
        for (auto it = m_objects.rbegin(); it != m_objects.rend(); it++) {
            if ((*it)->getDrawCount() >= nowCount)
                (*it)->draw(nowCount, scrollRate);
        }
    }
    double PlayNotesData::calDrawCount(const double currentCount) const
    {
        double drawCount = currentCount;
        for (auto&& elm : m_stopRanges) {
            if (currentCount >= elm.m_judgeCount) {
                drawCount -= std::min(elm.m_rangeCount, currentCount - elm.m_judgeCount);
            }

        }
        return drawCount;
    }

    BPMType PlayNotesData::getCurrentBPM() const
    {
        return m_tempoInfos[m_currentBarIndex].m_bar.getBPM();
    }
    double PlayNotesData::getLastBarCount() const
    {
        return m_notesData.getSheet().getLastBarCount();
    }
    const s3d::Color& PlayNotesData::getColor() const
    {
        return m_notesData.getColor();
    }
    s3d::String PlayNotesData::getLevelNameAndLevel() const
    {
        return m_notesData.getLevelWithStar() + U" - " + m_notesData.getLevelName();
    }
    MusicData PlayNotesData::getMusic() const
    {
        return m_notesData.getMusic();
    }
}
