#pragma once
#include <utility>
#include <Siv3D/String.hpp>
#include <Siv3D/Array.hpp>
#include <Siv3D/Color.hpp>
#include <core/Data/CourseData/CourceEntry.hpp>
#include <core/Data/Score/CourseScore.hpp>

namespace ct
{
	class CourseData
	{
	public:
		/// <summary>
		/// インデックスの初期化
		/// </summary>
		static void ResetIndex();
	public:
		CourseData() = default;
		CourseData(const s3d::String& path);

		bool load(const s3d::String& path);

		void saveScore(const CourseScore& score)const;

		bool canPlay() const;

		const s3d::Array<CourceEntry>& getEntries()const;
		size_t getEntrySize() const;
		const s3d::String& getTitle()const;
		const s3d::String& getFileName()const;
		const s3d::String& getGenre()const;

		size_t getIndex()const;

		s3d::String getScorePath() const;

		bool isClear() const;
		bool isMainClear() const;
		bool isLifeClear() const;
		const CourseScore& getScore()const;

		s3d::Color getStarColor() const;

		const s3d::Color& getColor() const;

		const CourceEntry& operator[](size_t index) const;
	private:
		class CourseHandle;
		std::shared_ptr<CourseHandle> m_handle;
	};
}
