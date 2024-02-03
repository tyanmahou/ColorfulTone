#pragma once
#include <core/Data/MusicData/MusicData.hpp>
#include <Siv3D/TextureAsset.hpp>
#include <functional>

namespace ct
{
	using RefinerType = std::function<bool(const MusicData&)>;

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
	class GenreData
	{
	private:
		GenreType m_genreType;	//ソート用
		int m_lv;			//レベル用
	public:
		s3d::String m_name;
		RefinerType m_refiner;

		GenreData(const GenreType type, const s3d::String& name, const RefinerType& refiner, int lv = 0) :
			m_genreType(type),
			m_name(name),
			m_refiner(refiner),
			m_lv(lv) {}

		s3d::TextureAsset getTexture()const
		{
			switch (m_genreType) {
			case GenreType::All:
				return s3d::TextureAsset(U"genre_all");
			case GenreType::Folder:
				return s3d::TextureAsset(U"genre_file");
			case GenreType::Lv:
				if (m_lv >= 12) {
					return s3d::TextureAsset(U"genre_level5");
				} else if (m_lv >= 9) {
					return s3d::TextureAsset(U"genre_level4");
				} else if (m_lv >= 7) {
					return s3d::TextureAsset(U"genre_level3");
				} else if (m_lv >= 4) {
					return s3d::TextureAsset(U"genre_level2");
				} else {
					return s3d::TextureAsset(U"genre_level1");
				}
			case GenreType::StarLv:
				return s3d::TextureAsset(U"genre_level_star");
			case GenreType::Favorite:
				return s3d::TextureAsset(U"genre_favorite");
			default:
				break;
			}
			return s3d::TextureAsset(U"genre_file");
		}
		const s3d::String& getName() const
		{
			return m_name;
		}
		bool operator < (const GenreData& right)const
		{
			if (m_genreType != right.m_genreType) {
				return m_genreType < right.m_genreType;
			}
			if (m_lv != right.m_lv) {
				return m_lv < right.m_lv;
			}
			return m_name < right.m_name;
		}
	};
}
