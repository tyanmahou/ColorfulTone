#pragma once
#include <core/Data/Finder/NotesFinder.hpp>
#include <core/Play/Session/PlayTrack.hpp>
#include <variant>

namespace ct
{
	/// <summary>
	/// コースエントリーの種類
	/// </summary>
	enum class CourceEntryKind
	{
		/// <summary>
		/// 通常
		/// </summary>
		Default,

		/// <summary>
		/// ランダム
		/// </summary>
		Random,
	};

	struct CourceEntryDefault
	{
		s3d::String path;
		MusicNotesIndex index;
	};

	struct CourceEntryRandom
	{
		CTCFReader condition;
	};

	/// <summary>
	/// コースのエントリー情報
	/// </summary>
	class CourceEntry
	{
	public:
		static CourceEntry CreateDefault(const s3d::String& path, const s3d::Optional<String>& detail);
		static CourceEntry CreateRandom(const s3d::String& condition, const s3d::Optional<String>& detail);
	public:
		bool canPlay()const
		{
			return m_canPlay;
		}

		PlayTrack choice() const;

		s3d::Texture getJucketTexture() const;
		s3d::ColorF getJucketColor() const;
		String getJucketTitle() const;
		String getJucketDetail() const;
	private:
		CourceEntryKind m_kind;
		std::variant<CourceEntryDefault, CourceEntryRandom> m_data;
		bool m_canPlay;

		s3d::Optional<String> m_detail;
	};
}