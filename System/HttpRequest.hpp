#pragma once
#include<Siv3D/JSONValue.hpp>
#include<memory>

class HttpRequest
{
public:
	enum class Status
	{
		None,
		Success,
		Failed,
	};
private:
	class Impl;
	std::shared_ptr<Impl> m_pImpl;
public:
	HttpRequest();
	HttpRequest(const s3d::URL& endPoint, const s3d::JSONObject& sendParams = {});
	~HttpRequest() = default;

	Status  request(const s3d::URL& endPoint, const s3d::JSONObject& sendParams = {})const;
	Status getStatus()const;
	const s3d::JSONValue& getResponse()const;

	bool isSuccess()const;
	bool isFailed()const;

	operator bool()const;
};

