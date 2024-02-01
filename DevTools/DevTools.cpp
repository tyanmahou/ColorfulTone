#include "DevTools.hpp"
#include <utils/Encode/EncodeUtil.hpp>

namespace ct::dev
{
	void DevTools::FixFileEncode()
	{
        Array<FilePath> updatedPaths;
        for (auto p : FileSystem::DirectoryContents(U"Music")) {
            if (!FileSystem::IsFile(p)) {
                continue;
            }
            if (auto ext = FileSystem::Extension(p); !(ext == U"ini" || ext == U"csv")) {
                continue;
            }
            if (EncodeUtil::ConverToUTF8(p)) {
                updatedPaths << p;
            }
        }
        for (auto p : FileSystem::DirectoryContents(U"Sample")) {
            if (!FileSystem::IsFile(p)) {
                continue;
            }
            if (auto ext = FileSystem::Extension(p); !(ext == U"ini" || ext == U"csv")) {
                continue;
            }
            if (EncodeUtil::ConverToUTF8(p)) {
                updatedPaths << p;
            }
        }
        for (auto p : FileSystem::DirectoryContents(U"Course")) {
            if (!FileSystem::IsFile(p)) {
                continue;
            }
            if (auto ext = FileSystem::Extension(p); !(ext == U"ctc")) {
                continue;
            }
            if (EncodeUtil::ConverToUTF8(p)) {
                updatedPaths << p;
            }
        }
        for (auto p : FileSystem::DirectoryContents(U"CustomFolder")) {
            if (!FileSystem::IsFile(p)) {
                continue;
            }
            if (auto ext = FileSystem::Extension(p); !(ext == U"ctfolder")) {
                continue;
            }
            if (EncodeUtil::ConverToUTF8(p)) {
                updatedPaths << p;
            }
        }
        Print << U"エンコード完了";
        //FilePath logFileName = U"log_encode_{}.txt"_fmt(DateTime::Now().format(U"yyyyMMddHHmmss"));
        if (auto savePath = Dialog::SaveFile({ FileFilter::Text() }, U"", U"エンコードログ保存")) {
            TextWriter log(*savePath);
            for (const FilePath& p : updatedPaths) {
                log.writeln(p);
            }
        }
	}
	void DevTools::ConvertPS()
    {
		Array<FilePath> paths = Dialog::OpenFiles({ FileFilter{  U"HLSL", { U"hlsl" }} }, FileSystem::CurrentDirectory(), U"コンパイルファイルPS");
		for (const FilePath& path : paths) {
			FilePath outPath = path.replaced(U"hlsl", U"ps");
			if (Platform::Windows::CompileHLSLToFile(path, outPath, ShaderStage::Pixel, U"PS")) {
				Print << U"OK << " << path;
			} else {
				Print << U"Error << " << path;
			}
		}
    }
}
