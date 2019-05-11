#include "DownloadContent.hpp"
#include <Siv3D.hpp>

#include "DownloadHistory.hpp"

namespace {
	Texture GetTexture(const URL& url)
	{
		HTTPClient client;
		ByteArray byte;
		client.download(url, byte);
		return Texture(std::move(byte));
	}
	DownloadContent::Type GetType(const String& type)
	{
		if (type == L"notes") {
			return DownloadContent::Type::Notes;
		}
		if (type == L"course") {
			return DownloadContent::Type::Course;
		}

		if (type == L"se") {
			return DownloadContent::Type::SE;
		}
		return DownloadContent::Type::Other;
	}
}
DownloadContent::DownloadContent(const s3d::JSONValue& json):
	m_downloadId(static_cast<uint32>(json[L"download_id"].getNumber())),
	m_title(json[L"title"].getString()),
	m_detail(json[L"detail"].getString()),
	m_type(::GetType(json[L"type"].getString())),
	m_downloadURL(),
	m_saveLocalPath(),
	m_isDownloaded(DownloadHistory::HasContein(m_downloadId))
{
	const auto& textureURL = json[L"texture_url"];
	if (!textureURL.isNull()) {
		m_texture = ::GetTexture(textureURL.getString());
	}
}

const s3d::String& DownloadContent::getTitle() const
{
	return m_title;
}

const s3d::String& DownloadContent::getDetail() const
{
	return m_detail;
}
