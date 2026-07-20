#include <core/Play/Session/Endless/PlayEndless.hpp>
#include <core/Play/Score/PlayingScore.hpp>
#include <core/Data/EndlessData/EndlessData.hpp>
#include <core/Play/Score/EndlessResult.hpp>
#include <core/Data/Score/ResultRank.hpp>
#include <scenes/Scene/SceneName.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace ct
{
    class PlayEndless::Impl
    {
    public:
        Impl(const EndlessData& endless, LifeGaugeKind gauge)
        {
            m_endless = endless;
            m_gauge = gauge;

            m_score = PlayingScore(gauge);
            m_endlessResult.init(m_gauge);

            m_trackIndex = 0;
            m_candidates = endless.candidates();
            for (size_t i = 0; i < 6; ++i) {
                m_playlist << choice();
            }
        }
        void next()
        {
            m_playlist << choice();
            ++m_trackIndex;
        }
        bool isEnd() const
        {
            return m_endlessResult.isEnd;
        }
        PlayTrack choice()
        {
            if (m_table.isEmpty()) {
                m_table = m_candidates.shuffled(); // シャッフルする
                if (m_table.size() > 1 && m_playlist.size() > 0 && m_playlist.back().index == m_table.back()) {
                    // 同じ譜面が出た場合はスキップ
                    m_table.pop_back();
                }
            }
            PlayTrack result{ 
                .index = m_table.back(),
                .isSecret = true,
            };
            m_table.pop_back();
            return result;
        }
        const EndlessData& getEndless() const
        {
            return m_endless;
        }
        const NotesData& getTrack() const
        {
            const auto& ids = m_playlist[m_trackIndex].index;
            return Game::Musics()[ids.first][ids.second];
        }
        const s3d::Array<PlayTrack>& getPlaylist() const
        {
            return m_playlist;
        }
        size_t getTrackIndex() const
        {
            return m_trackIndex;
        }
        LifeGaugeKind getGaugeKind() const
        {
            return m_gauge;
        }
        const PlayingScore& getScore() const
        {
            return m_score;
        }
        void updateScore(const PlayingScore& score)
        {
            m_score = score;
            m_result = ResultRank::CalcScore(m_score, getTrack().getTotalNotes());

            m_endlessResult.score.maxCombo = score.m_maxTotalCombo;
            if (m_score.m_life <= 0) {
                m_endlessResult.isEnd = true;
            } else {
                // クリア数+1
                ++m_endlessResult.score.clearCount;
            }
        }
        const ScoreModel& getResult() const
        {
            return m_result;
        }
        const EndlessResult& getEndlessResult() const
        {
            return m_endlessResult;
        }
    private:
        EndlessData m_endless;
        LifeGaugeKind m_gauge = LifeGaugeKind::Normal;

        size_t m_trackIndex = 0;

        Array<PlayTrack> m_playlist;
        Array<MusicNotesIndex> m_table;
        Array<MusicNotesIndex> m_candidates;

        PlayingScore m_score;

        ScoreModel m_result;
        EndlessResult m_endlessResult;
    };
    PlayEndless::PlayEndless(const EndlessData& endless, LifeGaugeKind gauge):
        m_pImpl(std::make_shared<Impl>(endless, gauge))
    {
    }
    void PlayEndless::next()
    {
        return m_pImpl->next();
    }
    bool PlayEndless::isEnd() const
    {
        return m_pImpl->isEnd();
    }
    s3d::Optional<s3d::String> PlayEndless::playlistName() const
    {
        return m_pImpl->getEndless().getTitle();
    }
    LifeGaugeKind PlayEndless::gauge() const
    {
        return  m_pImpl->getGaugeKind();
    }
    const NotesData& PlayEndless::getNotes() const
    {
        return m_pImpl->getTrack();
    }
    bool PlayEndless::canForceFinishMusicGame() const
    {
        return false;
    }
    const s3d::Array<PlayTrack>& PlayEndless::playlist() const
    {
        return m_pImpl->getPlaylist();
    }
    size_t PlayEndless::getTrackIndex() const
    {
        return m_pImpl->getTrackIndex();
    }
    const PlayingScore& PlayEndless::getScore() const
    {
        return m_pImpl->getScore();
    }
    bool PlayEndless::keepScore() const
    {
        return true;
    }
    void PlayEndless::updateScore(const PlayingScore& score)
    {
        return m_pImpl->updateScore(score);
    }
    const ScoreModel& PlayEndless::getResult() const
    {
        return m_pImpl->getResult();
    }
    const EndlessResult* PlayEndless::getEndlessResult() const
    {
        return &m_pImpl->getEndlessResult();
    }
    s3d::StringView PlayEndless::selectScene() const
    {
        return SceneName::EndlessSelect;
    }
    s3d::StringView PlayEndless::playlistScene() const
    {
        return SceneName::Playlist;
    }
    const EndlessData& PlayEndless::getEndless() const
    {
        return m_pImpl->getEndless();
    }
}