#pragma once
#include<memory>
class CourseData;
class NotesData;

class PlayCourse
{
private:
	class Impl;
	std::shared_ptr<Impl> m_pImpl;
public:
	PlayCourse();
	void init(const CourseData& course) const;
	void exit() const;

	bool isActive()const;

	size_t next() const;

	bool isStart() const;
	bool isEnd() const;

	const CourseData& getCourse() const;
	const NotesData& getCurrentNotes() const;
	size_t PlayCourse::getTrackOrder()const;
};

