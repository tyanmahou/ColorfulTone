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
	m_downloadURL(json[L"download_url"].getString()),
	m_saveLocalPath(json[L"save_local_pass"].getString()),
	m_isDownloaded(DownloadHistory::HasContein(m_downloadId))
{
	const auto& textureURL = json[L"texture_url"];
	if (!textureURL.isNull()) {
		m_texture = ::GetTexture(textureURL.getString());
	}
}

s3d::uint32 DownloadContent::getDownloadId() const
{
	return m_downloadId;
}

const s3d::String& DownloadContent::getTitle() const
{
	return m_title;
}

const s3d::String& DownloadContent::getDetail() const
{
	return m_detail;
}

const s3d::String& DownloadContent::getDownloadUrl() const
{
	return m_downloadURL;
}

const s3d::String& DownloadContent::getSaveLocalPath() const
{
	return m_saveLocalPath;
}

const Texture& DownloadContent::getTexture() const
{
	if (m_texture) {
		return m_texture;
	}
	static Texture tex(L"Resource/Img/MusicSelect/icon/dl_file.png");
	return tex;
}

const s3d::Color& DownloadContent::getColor() const
{
	if (m_texture) {
		return Palette::White;
	}
	static const std::unordered_map<Type, Color> map
	{
		{Type::Notes, Color(255, 207, 207)},
		{Type::Course,Color(207, 240, 255)},
		{Type::SE, Color(207, 255, 230)},
		{Type::Other, Color(247, 255, 207)},
	};
	return map.at(m_type);
}

bool DownloadContent::isDownloaded() const
{
	return m_isDownloaded;
}

void DownloadContent::setDownloaded(bool isDownloaded)
{
	m_isDownloaded = isDownloaded;
}

