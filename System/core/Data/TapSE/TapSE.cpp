#include <core/Data/TapSE/TapSE.hpp>
#include <Siv3D.hpp>
namespace
{
    const FilePathView g_defaultPerfectPath = U"TapSE/デフォルト/tapP.mp3";
    const FilePathView g_defaultGreatPath = U"TapSE/デフォルト/tapGR.mp3";
    const FilePathView g_defaultGoodPath = U"TapSE/デフォルト/tapGD.mp3";

    void RegisterTapSE(const String& assetName, FilePathView path, FilePathView defaultPath)
    {
        AudioAsset::Unregister(assetName);
        AudioAsset::Register({ assetName, {U"System"} }, path);
        if (!AudioAsset(assetName)) {
            AudioAsset::Unregister(assetName);
            AudioAsset::Register({ assetName, {U"System"} }, defaultPath);
        }
    }
}
namespace ct
{
    TapSE::TapSE() :
        m_name(U"none"),
        m_perfectSE(g_defaultPerfectPath),
        m_greatSE(g_defaultGreatPath),
        m_goodSE(g_defaultGoodPath)
    {
    }
    TapSE::TapSE(const s3d::FilePath& dirPath) :
        m_name(FileSystem::FileName(dirPath)),
        m_perfectSE(dirPath + U"/tapP.mp3"),
        m_greatSE(dirPath + U"/tapGR.mp3"),
        m_goodSE(dirPath + U"/tapGD.mp3")
    {}

    TapSE::TapSE(const s3d::String& name, const s3d::FilePath& pPath, const s3d::FilePath& grPath, const s3d::FilePath& gdPath) :
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
        ::RegisterTapSE(U"PERFECT", m_perfectSE, g_defaultPerfectPath);
        ::RegisterTapSE(U"GREAT", m_greatSE, g_defaultGreatPath);
        ::RegisterTapSE(U"GOOD", m_goodSE, g_defaultGoodPath);
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

}