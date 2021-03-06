#include "PlayCourse.hpp"
#include"CourseData.h"
#include "Useful.hpp"
namespace
{
	using State = PlayCourse::State;
}
class  PlayCourse::Impl
{
private:
	bool m_isActive = false;

	size_t m_nowCourseIndex = 0;

	size_t m_currentNotesIndex = 0;
	State m_state = State::None;
	CourseScore m_score;

	size_t m_rankAAACount = 0;
	size_t m_apCount = 0;

public:
	const CourseData& currentCourse()const
	{
		return Game::Courses()[m_nowCourseIndex];
	}
	void clear()
	{
		m_currentNotesIndex = 0;
		m_score = CourseScore();
		m_score.life = 100.0;
		m_rankAAACount = 0;
		m_apCount = 0;
	}
	void init(const CourseData & course)
	{
		m_isActive = true;
		m_nowCourseIndex = course.getIndex();
		m_state = State::Playing;
		this->clear();
	}
	void exit()
	{
		m_isActive = false;
		m_state = State::None;
		this->clear();
	}
	bool isActive() const
	{
		return m_isActive;
	}
	size_t getCurrentNotesIndex()const
	{
		return m_currentNotesIndex;
	}
	size_t next()
	{
		return ++m_currentNotesIndex;
	}
	bool isLastNotes()const
	{
		return m_currentNotesIndex + 1 >= this->currentCourse().getNotesIDs().size();
	}
	void updateScoreAndState(float addRate, float life)
	{
		m_score.life = life;

		m_score.totalRate += addRate;
		int tmpRate = m_score.totalRate * 100;
		m_score.totalRate = tmpRate / 100.0f;

		if (addRate >= 100.0f)
		{
			++m_apCount;
		}
		if (addRate >= 97.0f)
		{
			++m_rankAAACount;
		}
		if (life <= 0)
		{
			m_state = State::Failure;
		}
		else if (life > 0 && this->isLastNotes())
		{
			m_state = State::Success;
			m_score.isClear = true;
			if (m_apCount >= m_currentNotesIndex + 1)
			{
				m_score.special = CourseSpecialResult::AP;
			}
			else if (m_rankAAACount >= m_currentNotesIndex + 1)
			{
				m_score.special = CourseSpecialResult::RankAAA;
			}
		}
	}
	State getState()const
	{
		return m_state;
	}
	const NotesData& getCurrentNotes()const
	{
		const auto& ids = this->currentCourse().getNotesIDs()[m_currentNotesIndex];
		return Game::Musics()[ids.first][ids.second];
	}
	const CourseScore & getScore()const
	{
		return m_score;
	}
};

PlayCourse::PlayCourse() :
	m_pImpl(std::make_shared<Impl>())
{}

void PlayCourse::init(const CourseData & course) const
{
	m_pImpl->init(course);
}

void PlayCourse::exit() const
{
	m_pImpl->exit();
}

bool PlayCourse::isActive() const
{
	return m_pImpl->isActive();
}

size_t PlayCourse::next() const
{
	return m_pImpl->next();
}

bool PlayCourse::isEnd() const
{
	return m_pImpl->getState()&State::End;
}

bool PlayCourse::isSuccess() const
{
	return m_pImpl->getState() == State::Success;
}

bool PlayCourse::isFailure() const
{
	return m_pImpl->getState() == State::Failure;
}

const CourseData & PlayCourse::getCourse() const
{
	return m_pImpl->currentCourse();
}

const NotesData & PlayCourse::getCurrentNotes()const
{
	return m_pImpl->getCurrentNotes();
}

bool PlayCourse::isStart() const
{
	return m_pImpl->getCurrentNotesIndex() == 0;
}

size_t PlayCourse::getTrackIndex() const
{
	return m_pImpl->getCurrentNotesIndex();
}

size_t PlayCourse::getTrackOrder()const
{
	return this->getTrackIndex() + 1;
}

PlayCourse::State PlayCourse::getState() const
{
	return m_pImpl->getState();
}

const CourseScore & PlayCourse::getScore()const
{
	return m_pImpl->getScore();
}

void PlayCourse::updateScoreAndState(float addRate, float life) const
{
	m_pImpl->updateScoreAndState(addRate, life);
}
