#include <utils/Http/RestRequestor.hpp>
#include <Siv3D.hpp>
#include "RestRequestor.hpp"

namespace ct
{
    class RestRequestor::Impl
    {
    public:
        Status post(const s3d::URL& endPoint, const s3d::JSON& sendParams)
        {
            // インターネット接続をチェック
            if (!Network::IsConnected()) {
                return m_status = Status::None;
            }

            // リクエストパラメーター
            JSON send;
            send[U"params"] = sendParams;
            std::string src = Format(send).narrow();
            MemoryWriter writer;
            auto result = s3d::SimpleHTTP::Post(
                endPoint,
                {
                     {U"Content-Type", U"application/json"},
                },
                src.c_str(),
                src.length(),
                writer
                );
            if (!result.isOK()) {
                return m_status = Status::Failed;
            }
            m_response = s3d::JSON::Load(MemoryReader(writer.getBlob()));
            if (!m_response) {
                return m_status = Status::None;
            }
            const String& status = m_response[U"status"].getOr<String>(U"none");

            if (status == U"success") {
                return m_status = Status::Success;
            } else if (status == U"failed") {
                return m_status = Status::Failed;
            }
            return m_status = Status::None;
        }

        Coro::Fiber<Status> postAsync(const s3d::URL& endPoint, const s3d::JSON& sendParams)
        {
            // インターネット接続をチェック
            if (!Network::IsConnected()) {
                co_return m_status = Status::None;
            }

            // リクエストパラメーター
            JSON send;
            send[U"params"] = sendParams;
            std::string src = Format(send).narrow();
            AsyncHTTPTask task = s3d::SimpleHTTP::PostAsync(
                endPoint,
                {
                     {U"Content-Type", U"application/json"},
                },
                src.c_str(),
                src.length()
                );
            while (!task.isReady()) {
                co_yield{};
            }
            HTTPResponse result = task.getResponse();
            if (!result.isOK()) {
                co_return m_status = Status::Failed;
            }
            m_response = s3d::JSON::Load(MemoryReader(task.getBlob()));
            if (!m_response) {
                co_return m_status = Status::None;
            }
            const String& status = m_response[U"status"].getOr<String>(U"none");

            if (status == U"success") {
                co_return m_status = Status::Success;
            } else if (status == U"failed") {
                co_return m_status = Status::Failed;
            }
            co_return m_status = Status::None;
        }
        Status getStatus() const
        {
            return m_status;
        }

        JSON getResponse()const
        {
            return m_response[U"result"];
        }
    private:
        JSON m_response;
        Status m_status = Status::None;
    };
    RestRequestor RestRequestor::Post(const s3d::URL& endPoint, const s3d::JSON& sendParams)
    {
        RestRequestor requestor;
        requestor.post(endPoint, sendParams);
        return requestor;
    }
    RestRequestor::RestRequestor() :
        m_pImpl(std::make_shared<Impl>())
    {
    }
    RestRequestor::Status RestRequestor::post(const s3d::URL& endPoint, const s3d::JSON& sendParams) const
    {
        return m_pImpl->post(endPoint, sendParams);
    }
    Coro::Fiber<RestRequestor::Status> RestRequestor::postAsync(const s3d::URL& endPoint, const s3d::JSON& sendParams) const
    {
        return m_pImpl->postAsync(endPoint, sendParams);
    }
    RestRequestor::Status RestRequestor::getStatus() const
    {
        return m_pImpl->getStatus();
    }
    s3d::JSON RestRequestor::getResponse() const
    {
        return m_pImpl->getResponse();
    }
    bool RestRequestor::isSuccess() const
    {
        return this->getStatus() == Status::Success;
    }
    bool RestRequestor::isFailed() const
    {
        return this->getStatus() == Status::Success;
    }
    RestRequestor::operator bool() const
    {
        return this->isSuccess();
    }
}