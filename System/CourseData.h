#pragma once
#include<utility>
#include<Siv3D.hpp>

class CourseData
{
	//楽曲IDと譜面IDのセット
	using Data = std::pair<unsigned, unsigned>;
private:
	Array<Data> m_notesID;
	String m_title;	//コースタイトル
	String m_genre;	//ジャンル名
	int m_index;	//ID

	bool m_isClear = false;

	String m_fileName;

	bool m_canPlay = true;

	void serchNotes(const String& notePath);

public:
	static int Index;
	CourseData(const String& path)
	{
		m_index = Index++;
		this->load(path);
	}

	bool load(const String& path);

	void save(bool isClear = true);

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

	void setClear(bool isClear = true)
	{
		m_isClear = isClear;
	}

	bool isClear()const
	{
		return m_isClear;
	}
	int getIndex() {
		return m_index;
	}

	String getScorePath() const;
};
