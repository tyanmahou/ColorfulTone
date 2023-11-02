﻿#pragma once
#include <Siv3D/Fwd.hpp>
#include <Siv3D/Optional.hpp>
#include <Siv3D/String.hpp>
#include <Siv3D/Parse.hpp>
#include <memory>

namespace ct
{
    class MusicData;

    class CTCFReader
    {
        class Impl;
        std::shared_ptr<Impl> m_pImpl;
    public:
        CTCFReader() = default;
        CTCFReader(const s3d::FilePath& ctfolder);

        operator bool()const;

        s3d::Optional<s3d::String> getOption(const s3d::String& option) const;

        s3d::Optional<s3d::String> getTitle() const
        {
            return this->getOption(U"TITLE");
        }

        s3d::int32 getOrder() const
        {
            return s3d::ParseOpt<s3d::int32>(this->getOption(U"ORDER").value_or(U"0")).value_or(0);
        }

        bool expression(const MusicData& music)const;
    };
}