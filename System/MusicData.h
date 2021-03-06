#pragma once
#include"NotesData.h"

class MusicData 
{
public:
	using ABLoop = Vec2;

	static int Index;
private:

	String m_musicName;				//曲名
	String m_artistName;			//アーティスト名
	Optional<String> m_authority;	// 出典
	String m_genreName;				//ジャンル名
	Texture m_texture;				//ジャケ絵

	BPMType m_minbpm,m_maxbpm;	//bpm

	Mahou::SoundBar m_minBar, m_maxBar;//bpmのbeat

	ABLoop m_loop;					//ループ範囲

	Array<NotesData> m_notesDatas;	//譜面情報

	String m_fileName;		//ファイルの名前

	String m_soundNameID;	//アセットへのアクセス
	DateTime m_lastUpdateAt;//更新日時
	bool m_isFavorite = false; // お気に入り
	int m_index;

public:
		MusicData() = default;
		MusicData(const String& genreName,const String& dirPath, const String& iniPath);

		 const Texture& getTexture()const{ return m_texture;}

		Sound getSound()const { return SoundAsset(m_soundNameID); }
		const String& getMusicName()const { return m_musicName; }
		const String& getArtistName()const { return m_artistName; }
		const Optional<String>& getAuthority()const { return m_authority; }
		const String& getFileName()const { return m_fileName; }
		const String& getGenreName()const { return m_genreName; }
		const ABLoop& getLoopRange()const { return m_loop; }
		const DateTime& getLastUpdateAt()const { return m_lastUpdateAt; }

		const String getArtistAndAuthority() const;

		const int getBPM() const;

		const Mahou::SoundBar& getMinSoundBeat()const { return m_minBar; }
		const Mahou::SoundBar& getMaxSoundBeat()const { if (m_maxbpm == -1)return m_minBar; return m_maxBar; }
		const BPMType& getMinBPM()const { return m_minbpm; };
		const BPMType& getMaxBPM()const { if (m_maxbpm == -1)return m_minbpm; return m_maxbpm;};
		const String getFormattedBpm()const;

	    Array<NotesData>& getNotesData(){ return m_notesDatas; }
		const Array<NotesData>& getNotesData()const { return m_notesDatas; }

		const String& getSoundNameID()const { return m_soundNameID; }
		const int getIndex()const { return m_index; }

		const NotesData& operator [](uint32 level) const
		{
			return m_notesDatas[level];
		}
		NotesData& operator [](uint32 level)
		{
			return m_notesDatas[level];
		}

		// 曲の長さ
		const double getLengthSec()const;

		bool isFavorite()const;
		void setFavorite(bool isFavorite);
		String getFavoriteFilePath()const;

		void saveFavorite(bool isFavorite);
};
