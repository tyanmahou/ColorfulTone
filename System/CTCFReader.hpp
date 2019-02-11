#pragma once
#include<Siv3D/Fwd.hpp>
#include<Siv3D/Optional.hpp>
#include<Siv3D/String.hpp>
#include<memory>

class MusicData;

class CTCFReader
{
	class Impl;
	std::shared_ptr<Impl> m_pImpl;
public:
	CTCFReader() = default;
	CTCFReader(const s3d::FilePath& ctfolder);

	operator bool()const;

	s3d::Optional<s3d::String> getOption(const s3d::String& option) const;

	s3d::Optional<s3d::String> getTitle() const
	{
		return this->getOption(L"TITLE");
	}

	bool expression(const MusicData& music)const;
};

