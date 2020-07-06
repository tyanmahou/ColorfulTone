#pragma once
#include "FavoriteModel.hpp"
#include <Siv3D/String.hpp>

class FavoriteLoader
{
public:
	static FavoriteModel Load(const s3d::FilePath& path);
	static void Save(const s3d::FilePath& path, const FavoriteModel& favorite);
};