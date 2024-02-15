#include "DevTools.hpp"
#include <commons/Migration/MigrationUtil.hpp>
#include <core/Analysis/Analyzer.hpp>
#include <utils/Thread/Task.hpp>

namespace ct::dev
{
    double g_progress = 0;
    void DevTools::FixFileEncode()
    {
        Array<FilePath> updatedPaths = MigrationUtil::FixFileEncodeToUTF8();

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
    Coro::Fiber<void> DevTools::AnalyzeAsync()
    {
        g_progress = 0;
        auto path = Dialog::SelectFolder(U"Music");
        if (!path) {
            co_return;
        }
        auto files = FileSystem::DirectoryContents(*path).filter([](const FilePath& p) {
            const bool isOfficial = !U".*\\d{4}_\\d{2}"_re.match(p).isEmpty();
            return isOfficial && FileSystem::Extension(p) == U"csv";
            });
        struct Data
        {
            FilePath path;
            int32 level;
            AnalyzeResult result;
        };
        Array<Data> data;
        auto task = [&] {
            size_t done = 0;
            for (const FilePath& filePath : files) {
                SheetMusic sheet(filePath);

                data.push_back({
                    FileSystem::RelativePath(filePath, *path),
                    sheet.getLv(),
                    Analyzer::Analyze(sheet)
                    });
                ++done;
                g_progress = static_cast<double>(done) / static_cast<double>(files.size());
            }
            g_progress = 1.0;
            };
        co_await Thread::Task{ std::move(task) };

        // 保存
        {
            if (auto savePath = Dialog::SaveFile({ FileFilter::Text() }, U"", U"エンコードログ保存")) {
                TextWriter log(*savePath);
                log.writeln(U"Path, Lv, Rating, Ave, LocalAve, Stop, Bpm, Speed");
                log.writeln(U"============================");
                for (const Data& d : data) {
                    String ln = U"{}, {}, {}, {}, {}, {}, {}, {}"_fmt(d.path, d.level, d.result.rating, d.result.aveRating, d.result.localAveRating, d.result.stopRating, d.result.bpmRating, d.result.speedRating);
                    log.writeln(ln);
                }
                log.writeln(U"");
                log.writeln(U"Lv, Median, Ave, Min, Max");
                log.writeln(U"============================");
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
                std::map <int32, Statistics> statics;
                for (const Data& d : data) {
                    int32 lv = Min(d.level, 14);
                    int64 rating = d.result.rating;

                    statics[lv].data.push_back(rating);
                    if (statics[lv].min > rating) {
                        statics[lv].min = rating;
                    }
                    if (statics[lv].max < rating) {
                        statics[lv].max = rating;
                    }
                    statics[lv].sum += rating;
                }
                for (auto&& [lv, s] : statics) {
                    s.data.sort();
                    String ln = U"{}, {}, {}, {}, {}"_fmt(lv, s.median(), s.ave(), s.min, s.max);
                    log.writeln(ln);
                }
            }
        }
        Print << U"Complete";
    }
    double DevTools::LoadingProgress()
    {
        return g_progress;
    }
}
