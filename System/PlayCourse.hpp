#pragma once
#include<memory>
#include<Siv3D/Fwd.hpp>
class CourseScore;
class CourseData;
class NotesData;

class PlayCourse
{
public:
	enum State :s3d::uint8
	{
		None = 0,
		Playing = 1,
		Success = 2,
		Failure = 4,
		End = 6,
	};
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
	bool isSuccess()const;
	bool isFailure()const;
	void updateScoreAndState(float addRate, float life)const;
	const CourseData& getCourse() const;
	const NotesData& getCurrentNotes() const;

	size_t PlayCourse::getTrackIndex()const;
	size_t PlayCourse::getTrackOrder()const;
	State getState()const;

	const CourseScore& getScore() const;
};

