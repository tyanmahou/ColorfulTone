#pragma once
#include <core/Types.hpp>
#include <utils/Singleton/Singleton.hpp>
#include <Siv3D/ColorF.hpp>

namespace ct
{
    /// <summary>
    /// 色エフェクト
    /// </summary>
    class ColorFx : protected Singleton<ColorFx>
    {
        friend class Singleton<ColorFx>;
    public:
        /// <summary>
        /// リクエスト
        /// </summary>
        /// <param name="type"></param>
        static void Request(NoteType type);

        /// <summary>
        /// 色取得
        /// </summary>
        /// <returns></returns>
        static s3d::ColorF GetColor();
    private:
        ColorFx();
        ~ColorFx();
    private:
        class Impl;
        std::unique_ptr<Impl> m_pImpl;

    };
}