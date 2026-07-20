#pragma once
#include <Siv3D/String.hpp>
#include <Siv3D/Color.hpp>
#include <core/Data/Score/EndlessScore.hpp>
#include <core/Data/MusicNotesIndex.hpp>

namespace ct 
{
	class EndlessData
	{
	public:
		/// <summary>
		/// インデックスの初期化
		/// </summary>
		static void ResetIndex();
	public:
		EndlessData() = default;
		EndlessData(const s3d::String& path);

		bool load(const s3d::String& path);

		void saveScore(LifeGaugeKind gauge, const EndlessGaugeScore& score)const;

		bool canPlay() const;
		size_t getIndex()const;
		s3d::String getScorePath() const;

		s3d::Texture getTexture() const;
		const s3d::Color& getColor() const;
		const s3d::String& getTitle() const;
		s3d::String getDetail() const;

		const EndlessGaugeScore& getScore(LifeGaugeKind gauge) const;

		s3d::Array<MusicNotesIndex> candidates() const;
	private:
		class EndlessHandle;
		std::shared_ptr<EndlessHandle> m_handle;
	};
}