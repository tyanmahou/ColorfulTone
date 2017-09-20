#pragma once
#include<utility>
#include<Siv3D.hpp>

class CourseData
{
	//楽曲IDと譜面IDのセット
	using Data = std::pair<unsigned, unsigned>;
private:
	Array<Data> m_notesID;
	String m_title;

	bool m_isClear = false;

	String m_fileName;

	bool m_canPlay = true;

	void serchNotes(const String& notePath);

public:
	CourseData(const String& path)
	{
		this->load(path);
	}

	bool load(const String& path);

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

	void setClear(bool isClear = true)
	{
		m_isClear = isClear;
	}

	bool isClear()const
	{
		return m_isClear;
	}

};
