#include"CourseData.h"
#include"MusicData.h"
#include"Game.h"
#include "ScoreLoader.hpp"

size_t CourseData::Index = 0;

void CourseData::serchNotes(const String & notePath)
{
	auto& musics = Game::Musics();

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

	m_score = CourseScoreLoader::Load(this->getScorePath());

	//タイトル
	m_title = ini.getOr<String>(L"Data.TITLE", L"None");

	//譜面データのインデックス検索
	//譜面データ
	for (size_t i = 0; true; ++i)
	{
		String notePath = ini.get<String>(Format(L"Course.COURSE", i));
		if (notePath.isEmpty)
			break;
		else
		{
			m_actualSize = i + 1;
			this->serchNotes(notePath);
		}

	}

	if (m_notesID.size() == 0)
		m_canPlay = false;

	return true;
}

void CourseData::saveScore(const CourseScore& score)const
{
	CourseScoreLoader::Save(this->getScorePath(), score);
}

String CourseData::getScorePath() const
{
	return L"UserData/CourseScore/" + m_genre + L"/" + m_fileName + L".bin";
}

Color CourseData::getColor() const
{
	if (m_score.special == CourseSpecialResult::AP)
	{
		return Palette::Red;
	}
	else if (m_score.special == CourseSpecialResult::RankAAA)
	{
		return Palette::Yellow;
	}
	return Palette::White;
}
