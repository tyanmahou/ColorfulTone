﻿#pragma once
#include <utility>
#include <Siv3D/String.hpp>
#include <Siv3D/Array.hpp>
#include <Siv3D/Color.hpp>
#include <core/Data/CourseData/CourceEntry.hpp>

namespace ct
{
	enum class CourseSpecialResult : s3d::uint8
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
	private:
		s3d::Array<CourceEntry> m_entries;

		s3d::String m_title;	//コースタイトル
		s3d::String m_genre;	//ジャンル名
		size_t m_index;	//ID
		CourseScore m_score;

		s3d::String m_fileName;

		bool m_canPlay = true;
	public:
		static size_t Index;
		CourseData(const s3d::String& path)
		{
			m_index = Index++;
			this->load(path);
		}

		bool load(const s3d::String& path);

		void saveScore(const CourseScore& score)const;

		bool canPlay()const
		{
			return m_canPlay;
		}

		const s3d::Array<CourceEntry>& getEntries()const
		{
			return m_entries;
		}

		const s3d::String& getTitle()const
		{
			return m_title;
		}
		const s3d::String& getFileName()const
		{
			return m_fileName;
		}
		const s3d::String& getGenre()const
		{
			return m_genre;
		}

		size_t getIndex()const
		{
			return m_index;
		}

		s3d::String getScorePath() const;

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

		s3d::Color getColor() const;
	};
}
