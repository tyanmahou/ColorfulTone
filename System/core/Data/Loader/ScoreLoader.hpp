#pragma once
#include <core/Data/Score/Score.hpp>
#include <memory>

namespace ct
{
	class ScoreLoader
	{
	public:
		static ScoreModel Load(const s3d::FilePath& path);
		static void Save(const s3d::FilePath& path, const ScoreModel& newScore);
	};

	struct CourseScore;

	class CourseScoreLoader
	{
	public:
		static CourseScore Load(const s3d::FilePath& path);
		static void Save(const s3d::FilePath& path, const CourseScore& newScore);
	};
}
