#include"VideoAsset.h"

//--------------------------------------------------------------------------------
//�ÓI�����o�ϐ�
//--------------------------------------------------------------------------------

std::unordered_map<AssetName, VideoPlayer> Mahou::VideoAsset::m_map;

//--------------------------------------------------------------------------------
//�R���X�g���N�^
//--------------------------------------------------------------------------------
Mahou::VideoAsset::VideoAsset(const AssetName& name) :
	VideoPlayer(m_map.at(name)) {}

bool Mahou::VideoAsset::Register(const AssetName& name, const FilePath& path, bool loop)
{
	if (!m_map.count(name))
	{
		m_map.emplace(name, VideoPlayer(path, loop));
		return true;
	}
	return false;
}