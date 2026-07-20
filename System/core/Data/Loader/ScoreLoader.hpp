#pragma once
#include <core/Data/Score/Score.hpp>
#include <memory>

namespace ct
{
	class ScoreLoader
	{
	public:
		static ScoreModel Load(s3d::FilePathView path);
		static void Save(s3d::FilePathView path, const ScoreModel& newScore);
	};

	struct CourseScore;

	class CourseScoreLoader
	{
	public:
		static CourseScore Load(s3d::FilePathView path);
		static void Save(s3d::FilePathView path, const CourseScore& newScore);
	};

	struct EndlessScore;

	class EndlessScoreLoader
	{
	public:
		static EndlessScore Load(s3d::FilePathView path);
		static void Save(s3d::FilePathView path, const EndlessScore& newScore);
	};
}
