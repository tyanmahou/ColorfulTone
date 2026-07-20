#include <core/Play/Session/PlaySession.hpp>
#include <core/Data/NotesData/NotesData.hpp>
#include <core/Data/MusicData/MusicData.hpp>

namespace ct
{
    void PlaySession::exit()
    {
        m_impl = nullptr;
    }
    void PlaySession::next() const
    {
        m_impl->next();
    }
    bool PlaySession::isEnd() const
    {
        return m_impl->isEnd();
    }
    s3d::Optional<s3d::String> PlaySession::playlistName() const
    {
        return m_impl->playlistName();
    }
    LifeGaugeKind PlaySession::gauge() const
    {
        return m_impl->gauge();
    }
    bool PlaySession::isInvincible() const
    {
        return this->gauge() == LifeGaugeKind::Invincible;
    }
    bool PlaySession::isMainPassableGauge() const
    {
        return this->gauge() >= LifeGaugeKind::Normal;
    }
    const NotesData& PlaySession::getNotes() const
    {
        return m_impl->getNotes();
    }

    MusicData PlaySession::getMusic() const
    {
        return getNotes().getMusic();
    }
    bool PlaySession::canForceFinishMusicGame() const
    {
        return m_impl->canForceFinishMusicGame();
    }
    bool PlaySession::canChangeGauge() const
    {
        if (m_impl) {
            return false;
        }
        return true;
    }
    const s3d::Array<PlayTrack> PlaySession::playlist() const
    {
        if (!m_impl) {
            return s3d::Array<PlayTrack>();
        }
        return m_impl->playlist();
    }
    size_t PlaySession::getTrackIndex() const
    {
        if (!m_impl) {
            return 0;
        }
        return m_impl->getTrackIndex();
    }
    size_t PlaySession::getTrackOrder() const
    {
        return getTrackIndex() + 1;
    }
    bool PlaySession::isFristTrack() const
    {
        return getTrackIndex() == 0;
    }
    const PlayingScore& PlaySession::getScore() const
    {
        return m_impl->getScore();
    }
    bool PlaySession::keepScore() const
    {
        return m_impl->keepScore();
    }
    void PlaySession::updateScore(const PlayingScore& score) const
    {
        return m_impl->updateScore(score);
    }
    const ScoreModel& PlaySession::getResult() const
    {
        return m_impl->getResult();
    }
    const CourseResult* PlaySession::getCourseResult() const
    {
        return m_impl->getCourseResult();
    }
    const EndlessResult* PlaySession::getEndlessResult() const
    {
        return m_impl->getEndlessResult();
    }
    s3d::StringView PlaySession::selectScene() const
    {
        return m_impl->selectScene();
    }
    s3d::StringView PlaySession::playlistScene() const
    {
        return m_impl->playlistScene();
    }
}
