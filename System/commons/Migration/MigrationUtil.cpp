#include <commons/Migration/MigrationUtil.hpp>
#include <utils/Encode/EncodeUtil.hpp>
#include <Siv3D.hpp>
#include <cstdlib>

namespace ct
{
    Array<FilePath> MigrationUtil::FixFileEncodeToUTF8()
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
        return updatedPaths;
    }
    void MigrationUtil::MoveScoreFolder()
    {
        if (FileSystem::Exists(U"Score/CourseScore")) {
            FileSystem::Rename(U"Score/CourseScore", U"UserData/CourseScore");
        }
        if (FileSystem::Exists(U"Score")) {
            FileSystem::Rename(U"Score", U"UserData/Score");
        }
        if (FileSystem::Exists(U"System")) {
            FileSystem::Rename(U"System", U"UserData/System");
        }
    }
    bool Rename(FilePathView dir, FilePathView from, FilePathView to)
    {
        const FilePath fromPath = U"{}/{}"_fmt(dir, from);
        const FilePath toPath = U"{}/{}"_fmt(dir,to);
        return FileSystem::Rename(fromPath, toPath);
    }
    void MigrationUtil::RenameMusicFolder(FilePathView from, FilePathView to)
    {
        // Musicフォルダ
        Rename(U"Music", from, to);
        // Scoreフォルダ
        Rename(U"UserData/Score", from, to);
        // Favorite
        Rename(U"UserData/Favorite", from, to);

        // コースファイル修正
        for (const s3d::FilePath& path : FileSystem::DirectoryContents(U"Course")) {
            if (!FileSystem::IsFile(path)) {
                continue;
            }
            if (auto ext = FileSystem::Extension(path); !(ext == U"ctc")) {
                continue;
            }
            String text;
            {
                TextReader read(path);
                read.readAll(text);
            }
            if (!text.contains(from)) {
                continue;
            }
            {
                text.replace(from, to);
                TextWriter write(path);
                write.write(text);
            }
        }
    }
}
