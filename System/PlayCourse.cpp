#include "PlayCourse.hpp"
#include"CourseData.h"
#include "Useful.hpp"
class  PlayCourse::Impl
{
private:
	bool m_isActive = false;
	size_t m_currentNotesIndex = 0;
	size_t m_nowCourseIndex = 0;
public:
	CourseData& nowCourse()const
	{
		return Game::Courses()[m_nowCourseIndex];
	}
	void init(const CourseData & course)
	{
		m_nowCourseIndex = course.getIndex();
		m_currentNotesIndex = 0;
		m_isActive = true;
	}
	void exit()
	{
		m_isActive = false;
		m_currentNotesIndex = 0;
	}
	bool isActive() const
	{
		return m_isActive;
	}
	size_t getCurrentNotesIndex()
	{
		return m_currentNotesIndex;
	}
	size_t next()
	{
		return ++m_currentNotesIndex;
	}
	bool isEnd()const
	{
		return m_currentNotesIndex >= this->nowCourse().getNotesIDs().size();
	}
};

PlayCourse::PlayCourse():
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
	return m_pImpl->isEnd();
}

