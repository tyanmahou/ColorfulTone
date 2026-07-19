#pragma once
#include <Siv3D/String.hpp>
#include <Siv3D/Color.hpp>
#include <core/Data/EndlessData/EndlessScore.hpp>
#include <core/Data/Finder/NotesFinder.hpp>

namespace ct 
{
	class EndlessData
	{
	public:
		static size_t Index;
	public:
		EndlessData(const s3d::String& path);

		bool load(const s3d::String& path);

		void saveScore(const EndlessScore& score)const;

		bool canPlay()const
		{
			return m_canPlay;
		}
		size_t getIndex()const
		{
			return m_index;
		}

		s3d::String getScorePath() const;

		s3d::Texture getTexture() const;
		const s3d::ColorF& getColor() const;
		const s3d::String& getTitle() const;
		s3d::String getDetail() const;
	private:
		size_t m_index;	//ID
		s3d::String m_title;	// タイトル
		s3d::Optional<String> m_detail;	// 詳細
		s3d::String m_fileName;
		s3d::Color m_color;		// 色
		EndlessScore m_score;

		bool m_canPlay = true;
		CTCFReader m_condition;
	};
}