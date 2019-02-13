#include"VideoAsset.h"
#include<unordered_map>

//--------------------------------------------------------------------------------
//静的メンバ変数
//--------------------------------------------------------------------------------

std::unordered_map<s3d::AssetName, s3d::VideoPlayer> g_map;

//--------------------------------------------------------------------------------
//コンストラクタ
//--------------------------------------------------------------------------------
Mahou::VideoAsset::VideoAsset(const s3d::AssetName& name) :
	VideoPlayer(g_map.at(name)) {}

bool Mahou::VideoAsset::Register(const s3d::AssetName& name, const s3d::FilePath& path, bool loop)
{
	if (!g_map.count(name))
	{
		g_map.emplace(name, VideoPlayer(path, loop));
		return true;
	}
	return false;
}