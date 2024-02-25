#pragma once
#include <stop_token>
#include <Siv3D/Array.hpp>
#include <core/Data/Genre/GenreData.hpp>

namespace ct
{
    [[nodiscard]] s3d::Array<GenreData> LoadCustomFolder(const std::stop_token& stopToken);
}