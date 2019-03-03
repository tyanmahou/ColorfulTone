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
	size_t m_currentNotesIndex = 0;
	size_t m_nowCourseIndex = 0;
	State m_state = State::None;
	CourseScore m_score;
public:
	const CourseData& currentCourse()const
	{
		return Game::Courses()[m_nowCourseIndex];
	}
	void init(const CourseData & course)
	{
		m_isActive = true;
		m_nowCourseIndex = course.getIndex();
		m_currentNotesIndex = 0;
		m_state = State::Playing;
		m_score = {false, 0, 100.0};
	}
	void exit()
	{
		m_isActive = false;
		m_currentNotesIndex = 0;
		m_state = State::None;
		m_score = { false, 0, 100.0 };
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
		m_score.m_life = life;
		m_score.m_totalRate += addRate;
		if (life <= 0)
		{
			m_state = State::Failure;
		}
		else if (life > 0 && this->isLastNotes())
		{
			m_state = State::Success;
			m_score.m_isClear = true;
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
	return State();
}

const CourseScore & PlayCourse::getScore()const
{
	return m_pImpl->getScore();
}

void PlayCourse::updateScoreAndState(float addRate, float life) const
{
	m_pImpl->updateScoreAndState(addRate, life);
}
