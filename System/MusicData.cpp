#include"MusicData.h"
#include"GenreManager.h"

int MusicData::Index;
MusicData::MusicData(const String& genreName, const String& dirPath, const String& iniPath)
{
	m_index = Index;
	Index++;
	m_fileName = FileSystem::FileName(dirPath);
	m_lastUpdateAt = FileSystem::WriteTime(dirPath).value_or(DateTime::Now());
	m_genreName = genreName;

	INIReader iniReader(iniPath);
	if (!iniReader)
		return;
	//タイトル
	m_musicName.assign(iniReader.getOr<String>(L"Data.TITLE",L"None"));

	//ジャケ絵
	m_texture = Texture(dirPath + iniReader.get<String>(L"Data.IMG"));
	auto wavPath = dirPath + iniReader.get<String>(L"Data.MUSIC");

	//楽曲
	m_soundNameID = genreName + L"." + m_fileName;
	m_loop = iniReader.getOr<ABLoop>(L"Data.LOOP", ABLoop(0, 10));

	SoundAsset::Register(m_soundNameID, wavPath, SoundLoop(SecondsF(m_loop.x), SecondsF(m_loop.y)), { L"MusicData" });


	m_artistName= iniReader.getOr<String>(L"Data.ARTIST", L"None");
	m_minbpm=iniReader.getOr<BPMType>(L"Data.BPM", 120.0);
	m_maxbpm = iniReader.getOr<BPMType>(L"Data.MAXBPM", -1.0);

	m_minBar = Mahou::SoundBar(0, m_minbpm);
	m_maxBar = Mahou::SoundBar(0, m_maxbpm);

	m_bpm = EasingController<BPMType>(m_minbpm, m_maxbpm, Easing::Linear, 1000);
	//譜面データ
	for (int i = 0;true; ++i) 
	{
		String notePath =iniReader.get<String>(Format(L"Level.NOTES", i));
		if (notePath.isEmpty)
			break;
		else {
			if(FileSystem::Exists(dirPath+ notePath))
			m_notesDatas.emplace_back(genreName,dirPath, notePath);
		}
	}

}

