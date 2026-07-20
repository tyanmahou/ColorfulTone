#include <core/Play/Session/Free/PlayFree.hpp>
#include <core/Play/Score/PlayingScore.hpp>
#include <core/Data/NotesData/NotesData.hpp>
#include <core/Data/Score/ResultRank.hpp>
#include <scenes/Scene/SceneName.hpp>

namespace ct
{
    class PlayFree::Impl
    {
    public:
        Impl(const NotesData& notes, LifeGaugeKind gauge)
        {
            m_notes = notes;
            m_tracks << PlayTrack{
                .index = notes.getMusicIndex(),
            };
            m_score = PlayingScore(gauge);
        }
        bool isEnd() const
        {
            return m_isEnd;
        }
        LifeGaugeKind gauge() const
        {
            return m_gauge;
        }
        const NotesData& getNotes() const
        {
            return m_notes;
        }
        const s3d::Array<PlayTrack>& playlist() const
        {
            return m_tracks;;
        }
        const PlayingScore& getScore() const
        {
            return m_score;
        }
        void updateScore(const PlayingScore& score)
        {
            m_score = score;

            m_result = ResultRank::CalcScore(m_score, m_notes.getTotalNotes());
            m_isEnd = true;
        }
        const ScoreModel& getResult() const
        {
            return m_result;
        }
    private:
        NotesData m_notes;
        LifeGaugeKind m_gauge;
        s3d::Array<PlayTrack> m_tracks;

        PlayingScore m_score;

        ScoreModel m_result;
        bool m_isEnd = false;
    };
    PlayFree::PlayFree(const NotesData& notes, LifeGaugeKind gauge)
        :m_pImpl(std::make_shared<Impl>(notes, gauge))
    {
    }
    void PlayFree::next()
    {
    }
    bool PlayFree::isEnd() const
    {
        return m_pImpl->isEnd();
    }
    LifeGaugeKind PlayFree::gauge() const
    {
        return m_pImpl->gauge();
    }
    const NotesData& PlayFree::getNotes() const
    {
        return m_pImpl->getNotes();
    }
    bool PlayFree::canForceFinishMusicGame() const
    {
        return true;
    }
    const s3d::Array<PlayTrack>& PlayFree::playlist() const
    {
        return m_pImpl->playlist();
    }
    size_t PlayFree::getTrackIndex() const
    {
        return 0;
    }
    const PlayingScore& PlayFree::getScore() const
    {
        return m_pImpl->getScore();
    }
    bool PlayFree::keepScore() const
    {
        return false;
    }
    void PlayFree::updateScore(const PlayingScore& score)
    {
        return m_pImpl->updateScore(score);
    }
    const ScoreModel& PlayFree::getResult() const
    {
        return m_pImpl->getResult();
    }
    s3d::StringView PlayFree::selectScene() const
    {
        return SceneName::Select;
    }
    s3d::StringView PlayFree::playlistScene() const
    {
        return SceneName::Select;
    }
}
