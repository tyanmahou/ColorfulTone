#include <core/Play/PlayEndless.hpp>
#include <core/Data/EndlessData/EndlessData.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace ct
{
    class PlayEndless::Impl
    {
    public:
        Impl() = default;
        void init(const EndlessData& endless, LifeGaugeKind gauge)
        {
            clear();
            m_endless = endless;
            m_gauge = gauge;
            m_state = State::Playing;

            m_candidates = endless.candidates();

            for (size_t i = 0; i < 6; ++i) {
                m_selected << choice();
            }
        }
        void exit()
        {
            this->clear();
        }
        void clear()
        {
            m_currentNotesIndex = 0;
            m_state = State::None;

            m_selected.clear();
            m_table.clear();
            m_candidates.clear();
        }
        void next()
        {
            m_selected << choice();
            ++m_currentNotesIndex;
        }
        EndlessSelectedNotes choice()
        {
            if (m_table.isEmpty()) {
                m_table = m_candidates.shuffled(); // シャッフルする
                if (m_table.size() > 1 && m_selected.size() > 0 && m_selected.back().index == m_table.back()) {
                    // 同じ譜面が出た場合はスキップ
                    m_table.pop_back();
                }
            }
            EndlessSelectedNotes result{ .index = m_table.back() };
            m_table.pop_back();
            return result;
        }
        bool isActive() const
        {
            return m_state != State::None;
        }
        bool isStart() const
        {
            return m_currentNotesIndex == 0;
        }
        const EndlessData& getEndless() const
        {
            return m_endless;
        }
        const NotesData& getCurrentNotes() const
        {
            const auto& ids = m_selected[m_currentNotesIndex].index;
            return Game::Musics()[ids.first][ids.second];
        }
        const s3d::Array<EndlessSelectedNotes>& getSelectedNotes() const
        {
            return m_selected;
        }
        size_t getTrackIndex() const
        {
            return m_currentNotesIndex;
        }
        size_t getTrackOrder() const
        {
            return m_currentNotesIndex + 1;
        }
        LifeGaugeKind getGaugeKind() const
        {
            return m_gauge;
        }
    private:
        EndlessData m_endless;
        LifeGaugeKind m_gauge = LifeGaugeKind::Normal;

        size_t m_currentNotesIndex = 0;
        State m_state = State::None;

        Array<EndlessSelectedNotes> m_selected;
        Array<MusicNotesIndex> m_table;
        Array<MusicNotesIndex> m_candidates;
    };
    PlayEndless::PlayEndless() :
        m_pImpl(std::make_shared<Impl>())
    {
    }
    void PlayEndless::init(const EndlessData& endless, LifeGaugeKind gauge) const
    {
        m_pImpl->init(endless, gauge);
    }
    bool PlayEndless::isActive() const
    {
        return m_pImpl->isActive();
    }
    bool PlayEndless::isStart() const
    {
        return m_pImpl->isStart();
    }
    const EndlessData& PlayEndless::getEndless() const
    {
        return m_pImpl->getEndless();
    }
    const NotesData& PlayEndless::getCurrentNotes() const
    {
        return m_pImpl->getCurrentNotes();
    }
    const s3d::Array<EndlessSelectedNotes>& PlayEndless::getSelectedNotes() const
    {
        return m_pImpl->getSelectedNotes();
    }
    size_t PlayEndless::getTrackIndex() const
    {
        return m_pImpl->getTrackIndex();
    }
    size_t PlayEndless::getTrackOrder() const
    {
        return m_pImpl->getTrackOrder();
    }
    LifeGaugeKind PlayEndless::getGaugeKind() const
    {
        return m_pImpl->getGaugeKind();
    }
}