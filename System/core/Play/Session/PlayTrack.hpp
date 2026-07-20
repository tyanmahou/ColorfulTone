#pragma once
#include <core/Data/MusicNotesIndex.hpp>

namespace ct
{
	/// <summary>
	/// 選ばれた譜面
	/// </summary>
	struct PlayTrack
	{
		/// <summary>
		/// インデックス
		/// </summary>
		union
		{
			MusicNotesIndex index;
			struct
			{
				size_t musicIndex;
				size_t notesIndex;
			};
		};

		/// <summary>
		/// シークレットか
		/// </summary>
		bool isSecret;
	};
}
