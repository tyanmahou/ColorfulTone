#pragma once
#include <Siv3D/String.hpp>

namespace ct
{
    class TapSE
    {
    private:
        s3d::String m_name;

        s3d::FilePath m_perfectSE;
        s3d::FilePath m_greatSE;
        s3d::FilePath m_goodSE;
    public:
        TapSE();
        TapSE(const s3d::FilePath& dirPath);
        TapSE(const s3d::String& name, const s3d::FilePath& pPath, const s3d::FilePath& grPath, const s3d::FilePath& gdPath);

        void apply()const;

        const s3d::String& getName()const;

        const s3d::FilePath& getPerfectSE()const;
        const s3d::FilePath& getGreatSE()const;
        const s3d::FilePath& getGoodSE()const;

        FilePath getRelativePerfectSE() const;
        FilePath getRelativeGreatSE() const;
        FilePath getRelativeGoodSE() const;
    };
}
