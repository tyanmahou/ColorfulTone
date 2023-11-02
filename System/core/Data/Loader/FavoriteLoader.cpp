#include <core/Data/Loader/FavoriteLoader.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace ct
{
	FavoriteModel FavoriteLoader::Load(const s3d::FilePath& path)
	{
		FavoriteModel  ret;
		s3d::BinaryReader reader(path);
		if (!reader) {
			return ret;
		}

		reader.read<bool>(ret.isFavorite);
		return ret;
	}

	void FavoriteLoader::Save(const s3d::FilePath& path, const FavoriteModel& favorite)
	{
		s3d::BinaryWriter writer(path);

		if (!writer) {
			return;
		}

		writer.write(favorite.isFavorite);
	}
}
