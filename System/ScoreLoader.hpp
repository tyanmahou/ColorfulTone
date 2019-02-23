#pragma once
#include"Score.h"
#include<memory>

class ScoreLoader
{
public:
	static ScoreModel Load(const s3d::FilePath& path);
	static void Save(const s3d::FilePath& path, ScoreModel newScore);
};

