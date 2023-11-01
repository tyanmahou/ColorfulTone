#pragma once
#include <memory>
#include <Siv3D/URL.hpp>
#include <Siv3D/JSON.hpp>

namespace ct
{
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
		RestRequestor();
		RestRequestor(const s3d::URL& endPoint, const s3d::JSON& sendParams = {});
		~RestRequestor() = default;

		Status  request(const s3d::URL& endPoint, const s3d::JSON& sendParams = {})const;
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