#include "DevTools.hpp"
#include <commons/Migration/MigrationUtil.hpp>
#include <core/Analysis/Analyzer.hpp>
#include <utils/Thread/Task.hpp>
#include <utils/Math/StatisticsUtil.hpp>

namespace ct::dev
{
    bool DevTools::FixFileEncode()
    {
        Array<FilePath> updatedPaths = MigrationUtil::FixFileEncodeToUTF8();
        //FilePath logFileName = U"log_encode_{}.txt"_fmt(DateTime::Now().format(U"yyyyMMddHHmmss"));
        if (auto savePath = Dialog::SaveFile({ FileFilter::Text() }, U"", U"エンコードログ保存")) {
            TextWriter log(*savePath);
            if (!log) {
                return false;
            }
            for (const FilePath& p : updatedPaths) {
                log.writeln(p);
            }
        }
        return true;
    }
    bool DevTools::ConvertPS()
    {
        bool isSuccess = true;
        Array<FilePath> paths = Dialog::OpenFiles({ FileFilter{  U"HLSL", { U"hlsl" }} }, FileSystem::CurrentDirectory(), U"コンパイルファイルPS");
        for (const FilePath& path : paths) {
            FilePath outPath = path.replaced(U"hlsl", U"ps");
            if (Platform::Windows::CompileHLSLToFile(path, outPath, ShaderStage::Pixel, U"PS")) {
                isSuccess &= true;
            } else {
                isSuccess &= false;
            }
        }
        return isSuccess;
    }
    Coro::Fiber<bool> DevTools::AnalyzeAsync(bool isOfficialOnly)
    {
        auto path = Dialog::SelectFolder(U"Music");
        if (!path) {
            co_return false;
        }
        auto iniFiles = FileSystem::DirectoryContents(*path).filter([&](const FilePath& p) {
            const bool isOfficial = isOfficialOnly ? !U".*\\d{4}_\\d{2}"_re.match(p).isEmpty() : true;
            return isOfficial && FileSystem::Extension(p) == U"ini";
            });
        struct Data
        {
            FilePath path;
            int32 level;
            StarLv star;
            size_t noteCount;
            double noteSec;
            AnalyzeResult result;
        };
        Array<Data> data;
        auto task = [&] {
            size_t done = 0;
            for (const FilePath& iniPath : iniFiles) {
                INI ini(iniPath);
                if (ini) {
                    FilePath dirPath = FileSystem::ParentPath(iniPath);
                    //譜面データ
                    for (uint32 i = 0; true; ++i) {
                        String notePath = ini.get<String>(Format(U"Level.NOTES", i));
                        if (notePath.isEmpty())
                            break;
                        else {
                            FilePath noteFullPath = dirPath + notePath;
                            if (FileSystem::Exists(noteFullPath)) {

                                SheetMusic sheet(noteFullPath);

                                data.push_back({
                                    FileSystem::RelativePath(noteFullPath, *path),
                                    sheet.getLv(),
                                    sheet.getStarLv(),
                                    sheet.getTotalNotes(),
                                    sheet.getTotalNotesSec(),
                                    Analyzer::Analyze(sheet)
                                    });
                            }
                        }
                    }
                }
                ++done;
            }
            };
        co_await Thread::Task{ std::move(task) };

        // 保存
        {
            if (auto savePath = Dialog::SaveFile({ FileFilter::Text() }, U"", U"解析結果保存")) {
                {
                    TextWriter log(*savePath);
                    if (!log) {
                        co_return false;
                    }
                    log.writeln(U"Path, Lv, Star, Count, Time, Rating, Mean, Median, 80%Tile, 97%Tile, Max, NoteWeight, SpeedDev");
                    for (const Data& d : data) {
                        String ln = U"{}, {}, {}, {}, {:.2f}, {}, {}, {}, {}, {}, {}, {}, {}"_fmt(
                            d.path, 
                            d.level,
                            ToStr(d.star),
                            d.noteCount,
                            d.noteSec,
                            d.result.rating, 
                            d.result.meanRating, 
                            d.result.medianRating,
                            d.result.percentile80Rating,
                            d.result.percentile97Rating,
                            d.result.maxRating,
                            d.result.noteWeightRating,
                            d.result.speedDev
                        );
                        log.writeln(ln);
                    }
                    log.writeln(U"");
                    log.writeln(U"============================");
                    log.writeln(U"Lv, Median, Mean, Min, Max");
                    struct Statistics
                    {
                        Array<int64> data;

                        int64 min = std::numeric_limits<int64>::max();
                        int64 max = 0;
                        int64 sum = 0;

                        int64 median() const
                        {
                            size_t half = data.size() / 2;
                            if (data.size() % 2 == 0) {
                                return (data[half] + data[half - 1]) / 2;
                            } else {
                                return data[half];
                            }
                        }
                        int64 ave() const
                        {
                            return sum / data.size();
                        }
                    };
                    std::map<int32, Array<int64>> ratingMap;
                    for (const Data& d : data) {
                        int32 lv = d.level;
                        if (d.star > StarLv::None) {
                            lv = 13 + static_cast<int32>(d.star);
                        }
                        int64 rating = d.result.rating;

                        ratingMap[lv].push_back(rating);
                    }
                    for (auto&& [lv, d] : ratingMap) {
                        String ln = U"{}, {}, {}, {}, {}"_fmt(
                            lv < 14 ? Format(lv) : ToStr(static_cast<StarLv>(lv - 13)),
                            StatisticsUtil::Median<int64, int64>(d),
                            StatisticsUtil::Mean<int64, int64>(d),
                            StatisticsUtil::Min(d),
                            StatisticsUtil::Max(d)
                        );
                        log.writeln(ln);
                    }
                }
                s3d::System::LaunchFile(*savePath);
            }
        }
        co_return true;
    }
}
