#include "HttpRequest.hpp"
#include "JSONEncode.hpp"

#include <Siv3D/HTTPClient.hpp>
#include <Siv3D/ByteArray.hpp>
#include <Siv3D/JSONReader.hpp>
#include <Siv3D/Internet.hpp>

using namespace s3d;

class HttpRequest::Impl 
{
private:
	JSONReader m_response;
	Status m_status = Status::None;

public:
	Status request(const s3d::URL& endPoint, const s3d::JSONObject& sendParams)
	{
		// インターネット接続をチェック
		if (!Internet::IsConnected()) 
		{
			return m_status = Status::None;
		}

		HTTPClient client;

		// リクエストパラメーター
		JSONObject send;
		send[L"params"] = JSONValue(sendParams);

		auto content = JSON::Encode(JSONValue(std::move(send))).narrow();

		ByteArray responseByte;

		// Http通信
		client.requestPOST(
			endPoint,
			responseByte,
			L"",
			&content[0],
			content.length(),
			L"Content-Type: application/json"
		);

		m_response.open(std::move(responseByte));
		if (!m_response) {
			return m_status = Status::None;
		}
		const String& status = m_response[L"status"].getOr<String>(L"none");

		if (status == L"success")
		{
			return m_status = Status::Success;
		}
		else if (status == L"failed")
		{
			return m_status = Status::Failed;
		}
		return m_status = Status::None;
	}

	Status getStatus() const
	{
		return m_status;
	}

	const JSONValue& getResponse()const
	{
		return m_response[L"result"];
	}
};
HttpRequest::HttpRequest():
	m_pImpl(std::make_shared<Impl>())
{}

HttpRequest::HttpRequest(const s3d::URL & endPoint, const s3d::JSONObject & sendParams):
	HttpRequest()
{
	m_pImpl->request(endPoint, sendParams);
}

HttpRequest::Status HttpRequest::request(const s3d::URL& endPoint, const s3d::JSONObject& sendParams) const
{
	return m_pImpl->request(endPoint, sendParams);
}

HttpRequest::Status HttpRequest::getStatus() const
{
	return m_pImpl->getStatus();
}

const s3d::JSONValue& HttpRequest::getResponse() const
{
	return m_pImpl->getResponse();
}

bool HttpRequest::isSuccess() const
{
	return this->getStatus() == Status::Success;
}

bool HttpRequest::isFailed() const
{
	return this->getStatus() == Status::Success;
}

HttpRequest::operator bool() const
{
	return this->isSuccess();
}
