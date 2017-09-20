#pragma once
#include<functional>
#include"MusicData.h"
using RefinerType = std::function<bool(MusicData&)>;

enum GenreType
{
	All,
	Lv,
	MusicName,
	ArtistName,
	Folder,
	Custom,
};
class GenreData
{
private:
	GenreType m_genreType;	//�\�[�g�p
	int m_lv;			//���x���p
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
			switch (m_lv) 
			{
			case 3:
			case 4:return TextureAsset(L"genre_level2");
			case 5:
			case 6:return TextureAsset(L"genre_level3");
			case 7:
			case 8:return TextureAsset(L"genre_level4");
			default:
				if(m_lv>=9)
					return TextureAsset(L"genre_level5");
				else
					return TextureAsset(L"genre_level1");
				break;
			}
		default:
			break;
		}
		return  TextureAsset(L"genre_file");
	}
	bool operator < (const GenreData& right)const 
	{
		if (m_genreType == right.m_genreType)
		{
			if (m_genreType == Lv)
				return m_lv < right.m_lv;

			return m_name < right.m_name;
		}
		
		return m_genreType < right.m_genreType;
	}
};
