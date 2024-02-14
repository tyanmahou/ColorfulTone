#include <core/Data/MusicData/MusicData.hpp>
#include <core/Data/Loader/FavoriteLoader.hpp>
#include <utils/Audio/Loop.hpp>
#include <Siv3D.hpp>

namespace ct
{
	class MusicHandle : public std::enable_shared_from_this<MusicHandle>
	{
	public:
		inline static size_t Index = 0;
	public:
		void load(const s3d::String& genreName, const s3d::String& dirPath, const s3d::String& iniPath, bool isOfficial)
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
			m_loop = iniReader.getOr<MusicData::ABLoop>(U"Data.LOOP", MusicData::ABLoop(0, 10));

			AudioAsset::Register({ m_soundNameID,  { U"MusicData" } }, wavPath);


			m_artistName = iniReader.getOr<String>(U"Data.ARTIST", U"None");
			m_authority = iniReader.getOpt<String>(U"Data.AUTHORITY");
			m_minbpm = iniReader.getOr<BPMType>(U"Data.BPM", 120.0);
			m_maxbpm = iniReader.getOr<BPMType>(U"Data.MAXBPM", -1.0);

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
						m_notesDatas.emplace_back(shared_from_this(), dirPath, notePath, i);
					}
				}
			}
			if (latestDate) {
				m_lastUpdateAt = *latestDate;
			}
			// お気に入り
			m_isFavorite = FavoriteLoader::Load(this->getFavoriteFilePath()).isFavorite;
			m_isOfficial = isOfficial;
		}


		const s3d::Texture& getTexture()const { return m_texture; }

		s3d::Audio getSound()const { return s3d::AudioAsset(m_soundNameID); }
		const s3d::String& getMusicName()const { return m_musicName; }
		const s3d::String& getArtistName()const { return m_artistName; }
		const s3d::Optional<s3d::String>& getAuthority()const { return m_authority; }
		const s3d::String& getFileName()const { return m_fileName; }
		const s3d::String& getGenreName()const { return m_genreName; }
		const MusicData::ABLoop& getLoopRange()const { return m_loop; }
		const s3d::DateTime& getLastUpdateAt()const { return m_lastUpdateAt; }

		 s3d::String getArtistAndAuthority() const
		{
			//作曲家 + 出典
			String ret = m_artistName;
			if (m_authority.has_value() && !m_authority.value().isEmpty()) {
				ret += U" / " + m_authority.value();
			}
			return ret;
		}

		s3d::int32 getBPM() const
		{
			if (m_maxbpm == -1) {
				return static_cast<s3d::int32>(m_minbpm);
			}
			constexpr double timeMillisec = 1000;
			static Stopwatch stopwatch{ StartImmediately::Yes };
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

		SoundBar getMinSoundBeat()const { return SoundBar(0, m_minbpm); }
		SoundBar getMaxSoundBeat()const { if (m_maxbpm == -1)return SoundBar(0, m_minbpm); return SoundBar(0, m_maxbpm);}
		BPMType getMinBPM()const { return m_minbpm; };
		BPMType getMaxBPM()const { if (m_maxbpm == -1)return m_minbpm; return m_maxbpm; };

		const s3d::String getFormattedBpm()const
		{
			return U"BPM" + Pad(this->getBPM(), { 5,U' ' });
		}

		s3d::Array<NotesData>& getNotesData() { return m_notesDatas; }
		const s3d::Array<NotesData>& getNotesData()const { return m_notesDatas; }

		const s3d::String& getSoundNameID()const { return m_soundNameID; }
		size_t getIndex()const { return m_index; }
		// 曲の長さ
		double getLengthSec() const
		{
			return AudioAsset(this->m_soundNameID).lengthSec();
		}

		bool isFavorite() const
		{
			return m_isFavorite;
		}
		String getFavoriteFilePath() const
		{
			return U"UserData/Favorite/"
				+ this->getGenreName() + U"/"
				+ this->getFileName() + U"/"
				+ U"favorite.bin";
		}

		void saveFavorite(bool isFavorite)
		{
			m_isFavorite = isFavorite;
			FavoriteModel favorite;
			favorite.isFavorite = isFavorite;
			FavoriteLoader::Save(this->getFavoriteFilePath(), favorite);
		}

		bool isOfficial() const
		{
			return m_isOfficial;
		}
	private:
		s3d::String m_musicName;				//曲名
		s3d::String m_artistName;			    //アーティスト名
		s3d::Optional<s3d::String> m_authority;	// 出典
		s3d::String m_genreName;				//ジャンル名
		s3d::Texture m_texture;				    //ジャケ絵

		BPMType m_minbpm, m_maxbpm;         	//bpm

		MusicData::ABLoop m_loop;				//ループ範囲

		s3d::Array<NotesData> m_notesDatas;	    //譜面情報

		s3d::String m_fileName;		            //ファイルの名前

		s3d::String m_soundNameID;	            //アセットへのアクセス
		s3d::DateTime m_lastUpdateAt;           //更新日時
		bool m_isFavorite = false;              // お気に入り
		bool m_isOfficial = false;              // 公式譜面
		size_t m_index;
	};

	void MusicData::ResetIndex()
	{
		MusicHandle::Index = 0;
	}

	MusicData::MusicData(const std::shared_ptr<MusicHandle>& handle):
		m_handle(handle)
	{
	}

	MusicData::MusicData(const String& genreName, const String& dirPath, const String& iniPath, bool isOfficial):
		m_handle(std::make_shared<MusicHandle>())
	{
		m_handle->load(genreName, dirPath, iniPath, isOfficial);
	}
	const s3d::Texture& ct::MusicData::getTexture() const
	{
		return m_handle->getTexture();
	}
	s3d::Audio MusicData::getSound() const
	{
		return m_handle->getSound();
	}
	const s3d::String& MusicData::getMusicName() const
	{
		return m_handle->getMusicName();
	}
	const s3d::String& MusicData::getArtistName() const
	{
		return m_handle->getArtistName();
	}
	const s3d::Optional<s3d::String>& MusicData::getAuthority() const
	{
		return m_handle->getAuthority();
	}
	const s3d::String& MusicData::getFileName() const
	{
		return m_handle->getFileName();
	}
	const s3d::String& MusicData::getGenreName() const
	{
		return m_handle->getGenreName();
	}
	const MusicData::ABLoop& MusicData::getLoopRange() const
	{
		return m_handle->getLoopRange();
	}
	const s3d::DateTime& MusicData::getLastUpdateAt() const
	{
		return m_handle->getLastUpdateAt();
	}
	const s3d::String MusicData::getArtistAndAuthority() const
	{
		return m_handle->getArtistAndAuthority();
	}
	s3d::int32 MusicData::getBPM() const
	{
		return m_handle->getBPM();
	}
	SoundBar MusicData::getMinSoundBeat() const
	{
		return m_handle->getMinSoundBeat();
	}
	SoundBar MusicData::getMaxSoundBeat() const
	{
		return m_handle->getMaxSoundBeat();
	}
	BPMType MusicData::getMinBPM() const
	{
		return m_handle->getMinBPM();
	}
	BPMType MusicData::getMaxBPM() const
	{
		return m_handle->getMaxBPM();
	}
	s3d::String MusicData::getFormattedBpm() const
	{
		return m_handle->getFormattedBpm();
	}
	const s3d::String& MusicData::getSoundNameID() const
	{
		return m_handle->getSoundNameID();
	}
	size_t MusicData::getIndex() const
	{
		return m_handle->getIndex();
	}
	const double MusicData::getLengthSec() const
	{
		return m_handle->getLengthSec();
	}
	bool MusicData::isFavorite() const
	{
		return m_handle->isFavorite();
	}
	s3d::String MusicData::getFavoriteFilePath() const
	{
		return m_handle->getFavoriteFilePath();
	}
	void MusicData::saveFavorite(bool isFavorite)
	{
		return m_handle->saveFavorite(isFavorite);
	}
	bool MusicData::isOfficial() const
	{
		return m_handle->isOfficial();
	}
	s3d::Array<NotesData>& MusicData::getNotesData()
	{
		return m_handle->getNotesData();
	}
	const s3d::Array<NotesData>& MusicData::getNotesData() const
	{
		return m_handle->getNotesData();
	}
	const NotesData& MusicData::operator[](size_t level) const
	{
		return m_handle->getNotesData()[level];
	}
	NotesData& MusicData::operator[](size_t level)
	{
		return m_handle->getNotesData()[level];
	}
	MusicData::operator bool() const
	{
		return m_handle != nullptr;
	}
}
