#include "TapSE.hpp"
#include <Siv3D.hpp>
namespace
{
	const FilePath& g_defaultPerfectPath = L"TapSE/デフォルト/tapP.mp3";
	const FilePath& g_defaultGreatPath = L"TapSE/デフォルト/tapGR.mp3";
	const FilePath& g_defaultGoodPath = L"TapSE/デフォルト/tapGD.mp3";

	void RegisterTapSE(const String& assetName, const FilePath& path, const FilePath& defaultPath)
	{
		SoundAsset::Unregister(assetName);
		SoundAsset::Register(assetName, path, { L"System" });
		if (!SoundAsset(assetName)) {
			SoundAsset::Unregister(assetName);
			SoundAsset::Register(assetName, defaultPath, { L"System" });
		}
	}
}
TapSE::TapSE():
	m_name(L"none"),
	m_perfectSE(g_defaultPerfectPath),
	m_greatSE(g_defaultGreatPath),
	m_goodSE(g_defaultGoodPath)
{
}
TapSE::TapSE(const s3d::FilePath& dirPath):
	m_name(FileSystem::FileName(dirPath)),
	m_perfectSE(dirPath+L"/tapP.mp3"),
	m_greatSE(dirPath + L"/tapGR.mp3"),
	m_goodSE(dirPath + L"/tapGD.mp3")
{}

TapSE::TapSE(const s3d::String& name, const s3d::FilePath& pPath, const s3d::FilePath& grPath, const s3d::FilePath& gdPath):
	m_name(name),
	m_perfectSE(pPath),
	m_greatSE(grPath),
	m_goodSE(gdPath)
{}

const s3d::String& TapSE::getName() const
{
	return m_name;
}

void TapSE::apply() const
{
	::RegisterTapSE(L"PERFECT", m_perfectSE, g_defaultPerfectPath);
	::RegisterTapSE(L"GREAT", m_greatSE, g_defaultGreatPath);
	::RegisterTapSE(L"GOOD", m_goodSE, g_defaultGoodPath);
}

const s3d::FilePath& TapSE::getPerfectSE() const
{
	return m_perfectSE;
}

const s3d::FilePath& TapSE::getGreatSE() const
{
	return m_greatSE;
}

const s3d::FilePath& TapSE::getGoodSE() const
{
	return m_goodSE;
}
