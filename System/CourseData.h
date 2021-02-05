#pragma once
#include<utility>
#include<Siv3D.hpp>

enum class CourseSpecialResult: uint8
{
	None = 0,
	RankAAA = 1,
	AP = 2
};
struct CourseScore
{
	bool isClear = false;
	CourseSpecialResult special = CourseSpecialResult::None;
	float totalRate = 0.0;
	float life = 0.0;
};

class CourseData
{
public:
	//楽曲IDと譜面IDのセット
	using Data = std::pair<unsigned, unsigned>;
private:
	Array<Data> m_notesID;
	String m_title;	//コースタイトル
	String m_genre;	//ジャンル名
	size_t m_index;	//ID
	size_t m_actualSize; // 実際の譜面数
	CourseScore m_score;

	String m_fileName;

	bool m_canPlay = true;

	void serchNotes(const String& notePath);

public:
	static size_t Index;
	CourseData(const String& path)
	{
		m_index = Index++;
		this->load(path);
	}

	bool load(const String& path);

	void saveScore(const CourseScore& score)const;

	bool canPlay()const
	{
		return m_canPlay;
	}

	const Array<Data>& getNotesIDs()const
	{
		return m_notesID;
	}

	const String& getTitle()const
	{
		return m_title;
	}
	const String& getFileName()const
	{
		return m_fileName;
	}
	const String& getGenre()const
	{
		return m_genre;
	}

	int getIndex()const
	{
		return m_index;
	}

	String getScorePath() const;

	bool isClear() const
	{
		return m_score.isClear;
	}
	const CourseScore& getScore()const
	{
		return m_score;
	}
	void setScore(const CourseScore& score)
	{
		m_score = score;
	}

	Color getColor() const;
};
