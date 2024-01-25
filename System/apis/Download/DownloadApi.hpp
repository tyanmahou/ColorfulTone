#pragma once
#include <Siv3D/Array.hpp>
#include <utils/Coro/Fiber/Fiber.hpp>

namespace ct
{
	class DownloadContent;

	/// <summary>
	/// ダウンロードAPI
	/// </summary>
	class DownloadApi
	{
	public:
		/// <summary>
		/// ダウンロードリスト
		/// </summary>
		/// <returns></returns>
		static Coro::Fiber<s3d::Array<DownloadContent>> List();
	};
}
