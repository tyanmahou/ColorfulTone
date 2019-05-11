#pragma once
#include<Siv3D/Fwd.hpp>
#include<Siv3D/String.hpp>
#include<Siv3D/Texture.hpp>

class DownloadContent
{
public:
	enum class Type
	{
		Notes,
		Course,
		SE,
		Other,
	};
private:
	s3d::uint32 m_downloadId; // id
	s3d::String m_title;
	s3d::String m_detail;
	Type m_type;
	s3d::URL m_downloadURL; // downloadするurl
	s3d::FilePath m_saveLocalPath; // 保存するパス

	s3d::Texture m_texture;

	bool m_isDownloaded = false; // すでにダウンロード済みか
public:
	DownloadContent() = default;
	DownloadContent(const s3d::JSONValue& json);

	const s3d::String& getTitle() const;
	const s3d::String& getDetail() const;

};