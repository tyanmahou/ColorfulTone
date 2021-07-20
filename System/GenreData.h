#pragma once
#include<functional>
#include"MusicData.h"
using RefinerType = std::function<bool(MusicData&)>;

enum GenreType
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
	String m_name;
	RefinerType m_refiner;

	GenreData(const GenreType type,const String& name,const RefinerType& refiner,int lv=0) :
		m_genreType(type),
		m_name(name), 
		m_refiner(refiner),
		m_lv(lv) {}

	TextureAsset getTexture()const 
	{
		switch (m_genreType)
		{
		case GenreType::All:
			return TextureAsset(L"genre_all");
		case GenreType::Folder:
			return TextureAsset(L"genre_file");
		case GenreType::Lv:
			if (m_lv >= 12) {
				return TextureAsset(L"genre_level5");
			} else if (m_lv >= 9) {
				return TextureAsset(L"genre_level4");
			} else if (m_lv >= 7) {
				return TextureAsset(L"genre_level3");
			} else if (m_lv >= 4) {
				return TextureAsset(L"genre_level2");
			} else {
				return TextureAsset(L"genre_level1");
			}
		case GenreType::StarLv:
			return TextureAsset(L"genre_level_star");
		case GenreType::Favorite:
			return TextureAsset(L"genre_favorite");
		default:
			break;
		}
		return  TextureAsset(L"genre_file");
	}
	const String& getName() const
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
