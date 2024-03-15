#pragma once
#include <Siv3D/Types.hpp>

namespace ct
{
	enum class SortMode : s3d::uint8
	{
		FileName,
		MusicName,
		ArtistName,
		LastUpdateAt,
		Default = FileName
	};
	enum class AllNotesInfo : bool
	{
		Level,
		ClearRank
	};
	struct SelectMusicsInfo
	{
		size_t genre = 0;	// 選択中のジャンル
		size_t music = 0;	// 選択中の曲
		size_t level = 0;	// 選択中のレベル
		SortMode sortMode = SortMode::Default; // 選択中のソート
		AllNotesInfo notesInfo = AllNotesInfo::Level; //選択中の譜面表示情報

		static SelectMusicsInfo& Instance();
	};
}