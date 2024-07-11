#include "DevTools.hpp"
#include <commons/Migration/MigrationUtil.hpp>
#include <core/Analysis/Analyzer.hpp>
#include <core/Analysis/LvPredictor.hpp>
#include <utils/Thread/Task.hpp>
#include <utils/File/FileUtil.hpp>
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
    bool DevTools::ConvertVS()
    {
        bool isSuccess = true;
        Array<FilePath> paths = Dialog::OpenFiles({ FileFilter{  U"HLSL", { U"hlsl" }} }, FileSystem::CurrentDirectory(), U"コンパイルファイルVS");
        for (const FilePath& path : paths) {
            FilePath outPath = path.replaced(U"hlsl", U"vs");
            if (Platform::Windows::CompileHLSLToFile(path, outPath, ShaderStage::Vertex, U"VS")) {
                isSuccess &= true;
            } else {
                isSuccess &= false;
            }
        }
        return isSuccess;
    }
    Coro::Fiber<ProcessResult> DevTools::AnalyzeAsync(bool isOfficialOnly, bool isBuildModel)
    {
        LvPredictor::SetupIfNeed();

        auto path = Dialog::SelectFolder(U"Music");
        if (!path) {
            co_return {.status = ProcessResult::Status::Canceled};
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
            int32 pLevel;
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
                                auto analyzeResult = Analyzer::Analyze(sheet);
                                
                                data.push_back({
                                    FileSystem::RelativePath(noteFullPath, *path),
                                    sheet.getLv(),
                                    sheet.getStarLv(),
                                    sheet.getTotalNotes(),
                                    sheet.getTotalNotesSec(),
                                    analyzeResult,
                                    LvPredictor::Predict(analyzeResult.rating)
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
                        co_return{ 
                            .status = ProcessResult::Status::Failed,
                            .message = U"解析結果の保存に失敗しました"
                        };
                    }
                    log.writeln(U"No, Version, Title, File, Lv, Star, Count, Time, Rating, Mean, Median, 80%Tile, 97%Tile, Max, NoteWeight, pLv");
                    size_t no = 1;
                    for (const Data& d : data) {
                        auto sepPath = d.path.split(U'/');
                        String ln = U"{}, {}, {}, {}, {}, {}, {}, {:.2f}, {}, {}, {}, {}, {}, {}, {}, {}"_fmt(
                            no,
                            sepPath[0],
                            sepPath[1],
                            FileUtil::BaseName(sepPath[2]),
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
                            d.pLevel
                        );
                        log.writeln(ln);
                        ++no;
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
                    std::map<int32, Array<uint64>> ratingMap;
                    for (const Data& d : data) {
                        int32 lv = d.level;
                        if (d.star > StarLv::None) {
                            lv = 13 + static_cast<int32>(d.star);
                        }
                        uint64 rating = d.result.rating;

                        ratingMap[lv].push_back(rating);
                    }
                    for (auto&& [lv, d] : ratingMap) {
                        String ln = U"{}, {}, {}, {}, {}"_fmt(
                            lv < 16 ? Format(lv) : ToStr(static_cast<StarLv>(lv - 15)),
                            StatisticsUtil::Median<uint64, uint64>(d),
                            StatisticsUtil::Mean<uint64, uint64>(d),
                            StatisticsUtil::Min(d),
                            StatisticsUtil::Max(d)
                        );
                        log.writeln(ln);
                    }
                }
                s3d::System::LaunchFile(*savePath);
            }
        }
        if (isBuildModel) {
            if (auto savePath = Dialog::SaveFile({ FileFilter::CSV() }, U"", U"モデル保存")) {
                CSV model;
                for (const Data& d : data) {
                    int32 lv = d.level;
                    if (d.star > StarLv::None) {
                        if (d.star < StarLv::WhiteOne) {
                            continue;
                        }
                        // ★レベルを補正
                        if (d.star == StarLv::WhiteOne && !(lv >= 11 || lv <= 14)) {
                            lv = 14;
                        } else if (d.star == StarLv::WhiteTwo && lv != 15) {
                            lv = 15;
                        } else if (d.star == StarLv::WhiteThree && lv != 16) {
                            lv = 16;
                        } else if (d.star == StarLv::BlackOne && lv != 17) {
                            lv = 17;
                        } else if (d.star == StarLv::BlackTwo && lv != 18) {
                            lv = 18;
                        } else if (d.star == StarLv::BlackThree && lv != 19) {
                            lv = 19;
                        }
                        if (lv < 12 || 19 < lv) {
                            continue;
                        }
                    }
                    int64 rating = d.result.rating;

                    model.writeRow(rating, lv);
                }
                if (!model.save(*savePath)) {
                    co_return{
                        .status = ProcessResult::Status::Failed,
                        .message = U"モデルの保存に失敗しました"
                    };
                }
            }
        }
        co_return {
            .status = ProcessResult::Status::Completed,
            .message = U"譜面解析が完了しました"
        };
    }
}
