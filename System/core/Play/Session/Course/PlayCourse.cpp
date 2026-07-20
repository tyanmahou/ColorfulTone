#include <core/Play/Session/Course/PlayCourse.hpp>
#include <scenes/Scene/SceneName.hpp>
#include <core/Play/Score/PlayingScore.hpp>
#include <core/Play/Score/CourseResult.hpp>
#include <core/Data/Score/ResultRank.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace
{
	using namespace ct;
	using State = PlayCourse::State;
}

namespace ct
{
	class PlayCourse::Impl
	{
	public:
		Impl(const CourseData& course, LifeGaugeKind gauge)
		{
			m_nowCourseIndex = course.getIndex();
			m_gauge = gauge;

			m_trackIndex = 0;
			m_playlist.clear();
			for (const auto& entry : course.getEntries()) {
				m_playlist << entry.choice();
			}

			m_score = PlayingScore(gauge);
			m_courseResult.init(m_gauge, m_playlist.size());
		}
		size_t getTrackIndex()const
		{
			return m_trackIndex;
		}
		size_t next()
		{
			return ++m_trackIndex;
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

			const float life = ResultRank::CalcLifeRate(m_score);
			m_courseResult.updateScore(m_result.clearRate, life);
			if (life <= 0) {
				m_courseResult.end(false);
			} else if (life > 0 && this->isLastNotes()) {
				m_courseResult.end(true);
			}
		}
		const ScoreModel& getResult() const
		{
			return m_result;
		}
		const CourseResult& getCourseResult() const
		{
			return m_courseResult;
		}
		const CourseData& getCourse() const
		{
			return Game::Courses()[m_nowCourseIndex];
		}
	private:
		bool isLastNotes()const
		{
			return m_trackIndex + 1 >= m_playlist.size();
		}
	private:
		size_t m_nowCourseIndex = 0;
		LifeGaugeKind m_gauge = LifeGaugeKind::Normal;

		Array<PlayTrack> m_playlist;
		size_t m_trackIndex = 0;


		PlayingScore m_score;

		ScoreModel m_result;
		CourseResult m_courseResult{};
	};

	PlayCourse::PlayCourse(const CourseData& course, LifeGaugeKind gauge) :
		m_pImpl(std::make_shared<Impl>(course, gauge))
	{}
	void PlayCourse::next()
	{
		m_pImpl->next();
	}
	bool PlayCourse::isEnd() const
	{
		return m_pImpl->getCourseResult().isEnd;
	}
	s3d::Optional<s3d::String> PlayCourse::playlistName() const
	{
		return m_pImpl->getCourse().getTitle();
	}
	LifeGaugeKind PlayCourse::gauge() const
	{
		return m_pImpl->getGaugeKind();
	}

	const NotesData& PlayCourse::getNotes() const
	{
		return m_pImpl->getTrack();
	}
	bool PlayCourse::canForceFinishMusicGame() const
	{
		return false;
	}

	const s3d::Array<PlayTrack>& PlayCourse::playlist() const
	{
		return m_pImpl->getPlaylist();
	}

	size_t PlayCourse::getTrackIndex() const
	{
		return m_pImpl->getTrackIndex();
	}

	const PlayingScore& PlayCourse::getScore() const
	{
		return m_pImpl->getScore();
	}
	bool PlayCourse::keepScore() const
	{
		return true;
	}
	void PlayCourse::updateScore(const PlayingScore& score)
	{
		return m_pImpl->updateScore(score);
	}

	const ScoreModel& PlayCourse::getResult() const
	{
		return m_pImpl->getResult();
	}
	const CourseResult* PlayCourse::getCourseResult() const
	{
		return &m_pImpl->getCourseResult();
	}

	s3d::StringView PlayCourse::selectScene() const
	{
		return SceneName::CourseSelect;
	}

	s3d::StringView PlayCourse::playlistScene() const
	{
		return SceneName::Playlist;
	}
	const CourseData& PlayCourse::getCourse() const
	{
		return m_pImpl->getCourse();
	}

}
