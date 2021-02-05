#include "FavoriteLoader.hpp"
#include "Useful.hpp"

FavoriteModel FavoriteLoader::Load(const s3d::FilePath& path)
{
	FavoriteModel  ret;
	BinaryReader reader(path);
	if (!reader) {
		return ret;
	}

	reader.read<bool>(ret.isFavorite);
	return ret;
}

void FavoriteLoader::Save(const s3d::FilePath& path, const FavoriteModel& favorite)
{
	BinaryWriter writer(path);

	if (!writer) {
		return;
	}

	writer.write(favorite.isFavorite);
}
