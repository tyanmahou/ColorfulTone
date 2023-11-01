#pragma once
#include <commons/Secret/Secret.hpp>

namespace ct::Api
{
	static inline const s3d::String TitleVersion = Secret::API_ENDPOINT + U"title/version";
	static inline const s3d::String DownloadList = Secret::API_ENDPOINT + U"download/list";
}
