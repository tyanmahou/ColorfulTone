#include <Siv3D.hpp> 
#include "MainApp.hpp"

#include <utils/Encode/EncodeUtil.hpp>

void EncodeShiftJISToUTF8()
{
    for (auto p : FileSystem::DirectoryContents(U"Music")) {
        if (!FileSystem::IsFile(p)) {
            continue;
        }
        if (auto ext = FileSystem::Extension(p); !(ext == U"ini" || ext == U"csv")) {
            continue;
        }
        ct::EncodeUtil::ConvertShiftJISToUTF8(p);
    }
    for (auto p : FileSystem::DirectoryContents(U"Sample")) {
        if (!FileSystem::IsFile(p)) {
            continue;
        }
        if (auto ext = FileSystem::Extension(p); !(ext == U"ini" || ext == U"csv")) {
            continue;
        }
        ct::EncodeUtil::ConvertShiftJISToUTF8(p);
    }
    for (auto p : FileSystem::DirectoryContents(U"Course")) {
        if (!FileSystem::IsFile(p)) {
            continue;
        }
        if (auto ext = FileSystem::Extension(p); !(ext == U"ctc")) {
            continue;
        }
        ct::EncodeUtil::ConvertShiftJISToUTF8(p);
    }
    for (auto p : FileSystem::DirectoryContents(U"CustomFolder")) {
        if (!FileSystem::IsFile(p)) {
            continue;
        }
        if (auto ext = FileSystem::Extension(p); !(ext == U"ctfolder")) {
            continue;
        }
        ct::EncodeUtil::ConvertShiftJISToUTF8(p);
    }
}
void Main()
{
    // EncodeShiftJISToUTF8();
    ct::MainApp app;
    app.run();
}