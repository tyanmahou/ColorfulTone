#include"CourseData.h"
#include"Game.h"

int CourseData::Index = 0;

void CourseData::serchNotes(const String & notePath)
{
	auto& musics = Game::Instance()->m_musics;

	const String fileName = FileSystem::BaseName(notePath);
	const String dirPath = notePath.remove(FileSystem::FileName(notePath));

	unsigned musicIndex = 0;
	unsigned notesIndex = 0;

	for (auto&& m : musics)
	{
		const String mName = m.getGenreName() + L"/" + m.getFileName() + L"/";
		if (mName == dirPath)
		{
			for (auto&& notes : m.getNotesData())
			{
				if (notes.getFileName() == fileName)
				{
					m_notesID.emplace_back(musicIndex, notesIndex);
					return;
				}
				++notesIndex;
			}
		}
		++musicIndex;
	}

	m_canPlay = false;

}
bool CourseData::load(const String & path)
{
	INIReader ini(path);
	if (!ini)
		return false;

	m_fileName = FileSystem::BaseName(path);
	m_genre = FileSystem::FileName(FileSystem::ParentPath(path));

	BinaryReader saveReader(L"Score/CourseScore/" + m_genre + L"/" + m_fileName + L".bin");

	if (saveReader)
	{
		saveReader.read<bool>(m_isClear);
	}

	//タイトル
	m_title = ini.getOr<String>(L"Data.TITLE", L"None");

	//譜面データのインデックス検索
	//譜面データ
	for (int i = 0; true; ++i)
	{
		String notePath = ini.get<String>(Format(L"Course.COURSE", i));
		if (notePath.isEmpty)
			break;
		else
		{
			this->serchNotes(notePath);
		}

	}

	if (m_notesID.size() == 0)
		m_canPlay = false;

	return true;
}

void CourseData::save(bool isClear)
{
	BinaryWriter writer(L"Score/CourseScore/" + this->m_genre + L"/" + this->m_fileName + L".bin");

	writer.write(isClear);
}
