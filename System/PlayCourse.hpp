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

	bool isEnd() const;
	const NotesData& getCurrentNotes();
	bool isStart() const;
};

