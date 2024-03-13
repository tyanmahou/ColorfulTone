#pragma once
#include <utils/Coro/Fiber/Fiber.hpp>

namespace ct::dev
{
    struct ProcessResult
    {
        enum class Status
        {
            None,
            Completed,
            Failed,
            Canceled,
        };
        Status status{};
        s3d::String message;

        bool isCompleted() const
        {
            return status == Status::Completed;
        }
        bool isFailed() const
        {
            return status == Status::Failed;
        }
        bool isCanceled() const
        {
            return status == Status::Canceled;
        }
    };

    /// <summary>
    /// 開発ツール
    /// </summary>
    class DevTools
    {
    public:
        /// <summary>
        /// ファイルエンコードの修正
        /// </summary>
        static bool FixFileEncode();

        /// <summary>
        /// PixelShaderのコンバート
        /// </summary>
        static bool ConvertPS();

        /// <summary>
        /// VertexShaderのコンバート
        /// </summary>
        static bool ConvertVS();

        /// <summary>
        /// 解析
        /// </summary>
        static Coro::Fiber<ProcessResult> AnalyzeAsync(bool isOfficialOnly, bool isBuildModel = false);
    };
}
