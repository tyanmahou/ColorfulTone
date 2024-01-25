#pragma once
#include <memory>
#include <Siv3D/URL.hpp>
#include <Siv3D/JSON.hpp>
#include <utils/Coro/Fiber/Fiber.hpp>

namespace ct
{
	/// <summary>
	/// Restリクエスタ
	/// </summary>
	class RestRequestor
	{
	public:
		enum class Status
		{
			None,
			Success,
			Failed,
		};
	public:
		/// <summary>
		/// POSTでHTTPリクエストしてリクエスタを生成
		/// </summary>
		/// <param name="endPoint"></param>
		/// <param name="sendParams"></param>
		/// <returns></returns>
		static RestRequestor Post(const s3d::URL& endPoint, const s3d::JSON& sendParams = {});
	public:
		RestRequestor();
		~RestRequestor() = default;

		/// <summary>
		/// POSTでHTTPリクエスト
		/// </summary>
		/// <param name="endPoint"></param>
		/// <param name="sendParams"></param>
		/// <returns></returns>
		Status post(const s3d::URL& endPoint, const s3d::JSON& sendParams = {}) const;

		/// <summary>
		/// POSTでHTTPリクエスト 非同期
		/// </summary>
		/// <param name="endPoint"></param>
		/// <param name="sendParams"></param>
		/// <returns></returns>
		Coro::Fiber<Status> postAsync(const s3d::URL& endPoint, const s3d::JSON& sendParams = {}) const;

		Status getStatus()const;
		s3d::JSON getResponse()const;

		bool isSuccess()const;
		bool isFailed()const;

		explicit operator bool() const;
	private:
		class Impl;
		std::shared_ptr<Impl> m_pImpl;
	};
}