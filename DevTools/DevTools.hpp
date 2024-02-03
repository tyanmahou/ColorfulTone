#pragma once

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
    };
}
