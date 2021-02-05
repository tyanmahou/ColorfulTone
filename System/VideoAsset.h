#pragma once
#include<Siv3D/VideoPlayer.hpp>
#include<Siv3D/Asset.hpp>
namespace Mahou
{
	class VideoAsset:public s3d::VideoPlayer
	{
	public:
		VideoAsset(const s3d::AssetName& name);
		static bool Register(const s3d::AssetName& name, const s3d::FilePath& path, bool loop = true);

	};
}