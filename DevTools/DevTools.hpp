#pragma once
#include <utils/Coro/Fiber/Fiber.hpp>

namespace ct::dev
{
    /// <summary>
    /// 開発ツール
    /// </summary>
    class DevTools
    {
    public:
        /// <summary>
        /// ファイルエンコードの修正
        /// </summary>
        static void FixFileEncode();

        /// <summary>
        /// PixelShaderのコンバート
        /// </summary>
        static void ConvertPS();

        /// <summary>
        /// 解析
        /// </summary>
        static Coro::Fiber<void> AnalyzeAsync();

        static double LoadingProgress();
    };
}
