#pragma once
#include <core/Data/Genre/GenreFilter.hpp>
#include <core/Data/NotesData/StarLv.hpp>
#include <Siv3D/TextureAsset.hpp>
#include <functional>

namespace ct
{
	class CTCFReader;

	enum class GenreType
	{
		All,
		Lv,
		StarLv,
		MusicName,
		ArtistName,
		Folder,
		Custom,
		Favorite,
	};
	class GenreHandle;

	/// <summary>
	/// ジャンルデータ
	/// </summary>
	class GenreData
	{
	public:
		static GenreData CreateAll();
		static GenreData CreateFavorite();
		static GenreData CreateFolder(const s3d::String& folderName);
		static GenreData CreateLv(s3d::int32 lv);
		static GenreData CreateStarLv(StarLv starLv);
		static GenreData CreateCustom(const CTCFReader& ctcf);
	public:
		GenreData() = default;
		GenreData(const std::shared_ptr<GenreHandle>& handle);

		/// <summary>
		/// テクスチャの取得
		/// </summary>
		s3d::TextureAsset getTexture() const;

		/// <summary>
		/// 名前の取得
		/// </summary>
		/// <returns></returns>
		const s3d::String& getName() const;

		/// <summary>
		/// フィルタ条件
		/// </summary>
		const GenreFilter& getFilter() const;

		bool operator < (const GenreData& right) const;
	private:
		std::shared_ptr<GenreHandle> m_handle;
	};
}
