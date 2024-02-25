#include <core/Data/Genre/GenreData.hpp>
#include <core/Data/Genre/CTCFReader.hpp>
#include <Siv3D.hpp>

namespace ct
{
	class GenreHandle
	{
	public:
		GenreHandle(const GenreType type, const s3d::String& name, RefinerType&& refiner, s3d::int32 order = 0) :
			m_genreType(type),
			m_name(name),
			m_refiner(std::move(refiner)),
			m_order(order)
		{}

		s3d::TextureAsset getTexture() const
		{
			switch (m_genreType) {
			case GenreType::All:
				return s3d::TextureAsset(U"genre_all");
			case GenreType::Folder:
				return s3d::TextureAsset(U"genre_file");
			case GenreType::Lv:
				if (m_order >= 12) {
					return s3d::TextureAsset(U"genre_level5");
				} else if (m_order >= 9) {
					return s3d::TextureAsset(U"genre_level4");
				} else if (m_order >= 7) {
					return s3d::TextureAsset(U"genre_level3");
				} else if (m_order >= 4) {
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
		bool operator < (const GenreHandle& right)const
		{
			if (m_genreType != right.m_genreType) {
				return m_genreType < right.m_genreType;
			}
			if (m_order != right.m_order) {
				return m_order < right.m_order;
			}
			return m_name < right.m_name;
		}
		const RefinerType& getRefiner() const
		{
			return m_refiner;
		}
	private:
		s3d::String m_name;
		RefinerType m_refiner;
		GenreType m_genreType;  // ソート用
		s3d::int32 m_order;     // レベル用
	};

	GenreData GenreData::CreateAll()
	{
		return std::make_shared<GenreHandle>(
			GenreType::All, 
			U"ALL", 
			[]([[maybe_unused]] const MusicData& music)->bool {return true; }
		);
	}

	GenreData GenreData::CreateFavorite()
	{
		return std::make_shared<GenreHandle>(
			GenreType::Favorite, U"お気に入り",
			[](const MusicData& music)->bool {return  music.isFavorite(); }
		);
	}

	GenreData GenreData::CreateFolder(const s3d::String& folderName)
	{
		return std::make_shared<GenreHandle>(
			GenreType::Folder, 
			folderName, 
			[folderName](const MusicData& music)->bool {return music.getGenreName() == folderName; }
		);
	}

	GenreData GenreData::CreateLv(s3d::int32 lv)
	{
		return std::make_shared<GenreHandle>(
			GenreType::Lv, 
			Format(U"LEVEL:", lv), 
			[lv](const MusicData& music)->bool {
			return music.getNotesData().any([lv](const NotesData& notes) {return notes.getLevel() == lv; });
			}, 
			lv
		);
	}

	GenreData GenreData::CreateStarLv(StarLv starLv)
	{
		return std::make_shared<GenreHandle>(
			GenreType::StarLv, 
			Format(U"LEVEL:", ToStr(starLv)),
			[starLv](const MusicData& music)->bool {
				    return music.getNotesData().any([starLv](const NotesData& notes) {return notes.getStarLv() == starLv; });
			    },
			static_cast<int32>(starLv)
		);
	}

	GenreData GenreData::CreateCustom(const CTCFReader& ctcf)
	{
		String title = ctcf.getTitle().value_or(U"CustomFolder");
		return std::make_shared<GenreHandle>(
			GenreType::Custom, 
			title,
			[ctcf](const MusicData& music)->bool {return ctcf.expression(music); },
			ctcf.getOrder()
		);
	}

	GenreData::GenreData(const std::shared_ptr<GenreHandle>& handle):
        m_handle(handle)
    {
    }
	s3d::TextureAsset GenreData::getTexture() const
	{
		return m_handle->getTexture();
	}
	const s3d::String& GenreData::getName() const
	{
		return m_handle->getName();
	}
	const RefinerType& GenreData::getRefiner() const
	{
		return m_handle->getRefiner();
	}
	bool GenreData::operator<(const GenreData& right) const
	{
		if (!m_handle) {
			return true;
		} else if (!right.m_handle) {
			return false;
		}
		return (*m_handle) < (*right.m_handle);
	}
}
