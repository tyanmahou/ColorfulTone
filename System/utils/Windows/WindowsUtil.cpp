#include <utils/Windows/WindowsUtil.hpp>
#include <Siv3D/Windows/Windows.hpp>

namespace ct
{
    bool WindowsUtil::OpenExplore(s3d::FilePathView path)
    {
        HINSTANCE result = ::ShellExecute(NULL, L"explore", path.toWstr().c_str(), NULL, NULL, SW_SHOWNORMAL);
        return (intptr_t)result > 32;
    }
}
