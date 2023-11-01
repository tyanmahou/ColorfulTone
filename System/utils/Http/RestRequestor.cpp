#include <utils/Http/RestRequestor.hpp>
#include <Siv3D.hpp>

namespace ct
{
    class RestRequestor::Impl
    {
    public:
        Status request(const s3d::URL& endPoint, const s3d::JSON& sendParams)
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
    RestRequestor::RestRequestor() :
        m_pImpl(std::make_shared<Impl>())
    {
    }
    RestRequestor::RestRequestor(const s3d::URL& endPoint, const s3d::JSON& sendParams) :
        RestRequestor()
    {
        m_pImpl->request(endPoint, sendParams);
    }
    RestRequestor::Status RestRequestor::request(const s3d::URL& endPoint, const s3d::JSON& sendParams) const
    {
        return m_pImpl->request(endPoint, sendParams);
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