#include <core/Data/MusicData/MusicData.hpp>
#include <core/Data/Loader/FavoriteLoader.hpp>
#include <utils/Audio/Loop.hpp>
#include <Siv3D.hpp>
#include "MusicData.hpp"

namespace ct
{
	int MusicData::Index;
	MusicData::MusicData(const String& genreName, const String& dirPath, const String& iniPath)
	{
		m_index = Index;
		Index++;
		m_fileName = FileSystem::FileName(dirPath);
		m_lastUpdateAt = FileSystem::WriteTime(dirPath).value_or(DateTime::Now());
		m_genreName = genreName;

		INI iniReader(iniPath);
		if (!iniReader)
			return;
		//タイトル
		m_musicName.assign(iniReader.getOr<String>(U"Data.TITLE", U"None"));

		//ジャケ絵
		m_texture = Texture(dirPath + iniReader.get<String>(U"Data.IMG"));
		auto wavPath = dirPath + iniReader.get<String>(U"Data.MUSIC");

		//楽曲
		m_soundNameID = genreName + U"." + m_fileName;
		m_loop = iniReader.getOr<ABLoop>(U"Data.LOOP", ABLoop(0, 10));

		AudioAsset::Register({ m_soundNameID,  { U"MusicData" } }, wavPath);


		m_artistName = iniReader.getOr<String>(U"Data.ARTIST", U"None");
		m_authority = iniReader.getOpt<String>(U"Data.AUTHORITY");
		m_minbpm = iniReader.getOr<BPMType>(U"Data.BPM", 120.0);
		m_maxbpm = iniReader.getOr<BPMType>(U"Data.MAXBPM", -1.0);

		m_minBar = SoundBar(0, m_minbpm);
		m_maxBar = SoundBar(0, m_maxbpm);

		Optional<DateTime> latestDate;
		//譜面データ
		for (uint32 i = 0; true; ++i) {
			String notePath = iniReader.get<String>(Format(U"Level.NOTES", i));
			if (notePath.isEmpty())
				break;
			else {
				if (FileSystem::Exists(dirPath + notePath)) {
					auto writeTime = FileSystem::WriteTime(dirPath + notePath);
					if (!latestDate || *latestDate < *writeTime) {
						latestDate = writeTime;
					}
					m_notesDatas.emplace_back(this, dirPath, notePath, i);
				}
			}
		}
		if (latestDate) {
			m_lastUpdateAt = *latestDate;
		}
		// お気に入り
		m_isFavorite = FavoriteLoader::Load(this->getFavoriteFilePath()).isFavorite;
	}

	MusicData::MusicData(MusicData&& other) noexcept
	{
		*this = std::move(other);
	}

	MusicData& MusicData::operator=(MusicData&& other) noexcept
	{
		m_musicName = std::move(other.m_musicName);
		m_artistName = std::move(other.m_artistName);
		m_authority = std::move(other.m_authority);
		m_genreName = std::move(other.m_genreName);
		m_texture = std::move(other.m_texture);

		m_minbpm = other.m_minbpm;
		m_maxbpm = other.m_maxbpm;

		m_minBar = other.m_minBar;
		m_maxBar = other.m_maxBar;

		m_loop = other.m_loop;

		m_notesDatas = std::move(other.m_notesDatas);

		m_fileName = std::move(other.m_fileName);

		m_soundNameID = std::move(other.m_soundNameID);
		m_lastUpdateAt = other.m_lastUpdateAt;
		m_isFavorite = other.m_isFavorite;
		m_index = other.m_index;

		// MusicDataのインスタンスを差し替え
		for (auto& n : m_notesDatas) {
			n.setMusicPtr(this);
		}
		return *this;
	}

	const String MusicData::getArtistAndAuthority() const
	{
		//作曲家 + 出典
		String ret = m_artistName;
		if (m_authority.has_value() && !m_authority.value().isEmpty()) {
			ret += U" / " + m_authority.value();
		}
		return ret;
	}

	s3d::int32 MusicData::getBPM() const
	{
		if (m_maxbpm == -1) {
			return static_cast<s3d::int32>(m_minbpm);
		}
		constexpr double timeMillisec = 1000;
		static Stopwatch stopwatch{StartImmediately::Yes};
		static bool swapped = false;
		const double elapsed = Min<double>(stopwatch.ms(), timeMillisec) / timeMillisec;

		if (stopwatch.ms() >= 4000) {
			stopwatch.restart();
			swapped = !swapped;
		}
		if (swapped) {
			return static_cast<s3d::int32>(EaseInOut(Easing::Linear, m_maxbpm, m_minbpm, elapsed));
		}
		return static_cast<s3d::int32>(EaseInOut(Easing::Linear, m_minbpm, m_maxbpm, elapsed));
	}

	const String MusicData::getFormattedBpm() const
	{
		return U"BPM" + Pad(this->getBPM(), { 5,U' ' });
	}

	const double MusicData::getLengthSec() const
	{
		return AudioAsset(this->m_soundNameID).lengthSec();
	}

	bool MusicData::isFavorite() const
	{
		return m_isFavorite;
	}

	void MusicData::setFavorite(bool isFavorite)
	{
		m_isFavorite = isFavorite;
	}

	String MusicData::getFavoriteFilePath() const
	{
		return U"UserData/Favorite/"
			+ this->getGenreName() + U"/"
			+ this->getFileName() + U"/"
			+ U"favorite.bin";
	}

	void MusicData::saveFavorite(bool isFavorite)
	{
		m_isFavorite = isFavorite;
		FavoriteModel favorite;
		favorite.isFavorite = isFavorite;
		FavoriteLoader::Save(this->getFavoriteFilePath(), favorite);
	}
}
