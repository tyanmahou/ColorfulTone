#pragma once
#include <utility>
#include <Siv3D/String.hpp>
#include <Siv3D/Array.hpp>
#include <Siv3D/Color.hpp>

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
	public:
		//楽曲IDと譜面IDのセット
		using Data = std::pair<unsigned, unsigned>;
	private:
		s3d::Array<Data> m_notesID;
		s3d::String m_title;	//コースタイトル
		s3d::String m_genre;	//ジャンル名
		size_t m_index;	//ID
		size_t m_actualSize; // 実際の譜面数
		CourseScore m_score;

		s3d::String m_fileName;

		bool m_canPlay = true;

		void serchNotes(const s3d::String& notePath);

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

		const s3d::Array<Data>& getNotesIDs()const
		{
			return m_notesID;
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
