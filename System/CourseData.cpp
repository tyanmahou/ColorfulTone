#include"CourseData.h"
#include"Game.h"
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

	BinaryReader saveReader(L"Score/CourseScore/" + m_fileName + L".bin");


	if (saveReader)
	{
		saveReader.read<bool>(m_isClear);
		//unsigned int ui_spResult;
		//saveReader.read<unsigned int>(ui_spResult);
		//m_specialResult = static_cast<SpecialResult>(ui_spResult);
		//saveReader.read<float>(m_clearRate);
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
