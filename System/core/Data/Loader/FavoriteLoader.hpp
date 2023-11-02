#pragma once
#include <core/Data/Favorite/FavoriteModel.hpp>
#include <Siv3D/String.hpp>

namespace ct
{
	class FavoriteLoader
	{
	public:
		static FavoriteModel Load(const s3d::FilePath& path);
		static void Save(const s3d::FilePath& path, const FavoriteModel& favorite);
	};
}
