#pragma once
#include"Game.h"
namespace Mahou
{
	class VideoAsset:public VideoPlayer
	{
	private:
		static std::unordered_map<AssetName, VideoPlayer> m_map;
	public:
		VideoAsset(const AssetName& name);
		static bool Register(const AssetName& name, const FilePath& path, bool loop = true);

	};
}