#include <core/Data/DownloadContent/DownloadContent.hpp>
#include <core/Data/DownloadContent/DownloadHistory.hpp>
#include <Siv3D.hpp>

namespace {
	using namespace ct;

	Texture GetTexture(const URL& url)
	{
		MemoryWriter writer;
		SimpleHTTP::Load(url, writer);
		return Texture(MemoryReader(writer.getBlob()));
	}
	DownloadContent::Type GetType(const String& type)
	{
		if (type == U"notes") {
			return DownloadContent::Type::Notes;
		}
		if (type == U"course") {
			return DownloadContent::Type::Course;
		}

		if (type == U"se") {
			return DownloadContent::Type::SE;
		}
		return DownloadContent::Type::Other;
	}
}

namespace ct
{
	DownloadContent::DownloadContent(const s3d::JSON& json) :
		m_downloadId(json[U"download_id"].get<uint32>()),
		m_title(json[U"title"].getString()),
		m_detail(json[U"detail"].getString()),
		m_type(::GetType(json[U"type"].getString())),
		m_downloadURL(json[U"download_url"].getString()),
		m_saveLocalPath(json[U"save_local_pass"].getString()),
		m_isDownloaded(DownloadHistory::HasContein(m_downloadId))
	{
		const auto& textureURL = json[U"texture_url"];
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

	Texture DownloadContent::getTexture() const
	{
		if (m_texture) {
			return m_texture;
		}
		return TextureAsset(U"dl_file");
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
}
