#include "ScoreLoader.hpp"
#include "NotesData.h"

ScoreModel ScoreLoader::Load(const s3d::FilePath & path)
{
	ScoreModel ret;
	BinaryReader reader(path);
	if (!reader)
	{
		ret;
	}

	reader.read<bool>(ret.isClear);
	unsigned int ui_spResult;
	reader.read<unsigned int>(ui_spResult);
	ret.specialResult = static_cast<SpecialResult>(ui_spResult);
	reader.read<float>(ret.clearRate);

	return ret;
}

void ScoreLoader::Save(const s3d::FilePath & path, ScoreModel newScore)
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
