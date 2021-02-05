#include "ScoreLoader.hpp"
#include "NotesData.h"
#include "CourseData.h"
ScoreModel ScoreLoader::Load(const s3d::FilePath & path)
{
	ScoreModel ret;
	BinaryReader reader(path);
	if (!reader)
	{
		return ret;
	}

	reader.read<bool>(ret.isClear);
	unsigned int ui_spResult;
	reader.read<unsigned int>(ui_spResult);
	ret.specialResult = static_cast<SpecialResult>(ui_spResult);
	reader.read<float>(ret.clearRate);

	return ret;
}

void ScoreLoader::Save(const s3d::FilePath & path, const ScoreModel& newScore)
{
	BinaryWriter writer(path);

	if (!writer)
	{
		return;
	}

	writer.write(newScore.isClear);
	writer.write(static_cast<unsigned int>(newScore.specialResult));
	writer.write(newScore.clearRate);
}

CourseScore CourseScoreLoader::Load(const s3d::FilePath & path)
{
	CourseScore ret;
	BinaryReader reader(path);
	if (!reader)
	{
		return ret;
	}

	reader.read<bool>(ret.isClear);
	uint8 ui_sp;
	if (reader.read<uint8>(ui_sp))
	{
		if (ui_sp >= 3) {
			// 無効
			ui_sp = 0;
		}
		ret.special = static_cast<CourseSpecialResult>(ui_sp);
	}
	else
	{
		ret.special = CourseSpecialResult::None;
	}
	if (!reader.read<float>(ret.totalRate))
	{
		ret.totalRate = 0.0;
	}
	if (!reader.read<float>(ret.life))
	{
		ret.life = 0.0;
	}
	return ret;
}

void CourseScoreLoader::Save(const s3d::FilePath & path, const CourseScore & newScore)
{
	BinaryWriter writer(path);

	if (!writer)
	{
		return;
	}

	writer.write(newScore.isClear);
	writer.write(static_cast<uint8>(newScore.special));
	writer.write(newScore.totalRate);
	writer.write(newScore.life);
}
