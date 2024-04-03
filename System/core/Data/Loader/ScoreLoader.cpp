#include <core/Data/Loader/ScoreLoader.hpp>
#include <core/Data/NotesData/NotesData.hpp>
#include <core/Data/CourseData/CourseData.hpp>
#include <Siv3D.hpp>

namespace ct
{
	ScoreModel ScoreLoader::Load(const s3d::FilePath& path)
	{
		ScoreModel ret;
		BinaryReader reader(path);
		if (!reader) {
			return ret;
		}

		reader.read<bool>(ret.isClear);
		uint32 ui_spResult;
		reader.read<uint32>(ui_spResult);
		ret.specialResult = static_cast<SpecialResult>(ui_spResult);
		reader.read<float>(ret.clearRate);

		reader.read<bool>(ret.isLifeClear);
		int32 g;
		reader.read<int32>(g);
		ret.gauge = static_cast<LifeGaugeKind>(g);
		if (!ret.isLifeClear) {
			ret.gauge = LifeGaugeKind::None;
		}
		return ret;
	}

	void ScoreLoader::Save(const s3d::FilePath& path, const ScoreModel& newScore)
	{
		BinaryWriter writer(path);

		if (!writer) {
			return;
		}

		writer.write(newScore.isClear);
		writer.write(static_cast<uint32>(newScore.specialResult));
		writer.write(newScore.clearRate);
		writer.write(newScore.isLifeClear);
		writer.write(static_cast<int32>(newScore.gauge));
	}

	CourseScore CourseScoreLoader::Load(const s3d::FilePath& path)
	{
		CourseScore ret;
		BinaryReader reader(path);
		if (!reader) {
			return ret;
		}

		reader.read<bool>(ret.isClear);
		uint8 ui_sp;
		if (reader.read<uint8>(ui_sp)) {
			if (ui_sp >= 3) {
				// 無効
				ui_sp = 0;
			}
			ret.special = static_cast<CourseSpecialResult>(ui_sp);
		} else {
			ret.special = CourseSpecialResult::None;
		}
		if (!reader.read<float>(ret.totalRate)) {
			ret.totalRate = 0.0;
		}
		if (!reader.read<float>(ret.life)) {
			ret.life = 0.0;
		}
		return ret;
	}

	void CourseScoreLoader::Save(const s3d::FilePath& path, const CourseScore& newScore)
	{
		BinaryWriter writer(path);

		if (!writer) {
			return;
		}

		writer.write(newScore.isClear);
		writer.write(static_cast<uint8>(newScore.special));
		writer.write(newScore.totalRate);
		writer.write(newScore.life);
	}
}
