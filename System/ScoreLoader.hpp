#pragma once
#include"Score.h"
#include<memory>

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

