#include <core/Data/Genre/GenreData.hpp>
#include <core/Data/Genre/CTCFReader.hpp>
#include <core/Data/MusicData/MusicData.hpp>
#include <Siv3D.hpp>

namespace ct
{
	class GenreHandle
	{
	public:
		GenreHandle(const GenreType type, const s3d::String& name, GenreFilter&& filter, s3d::int32 order = 0) :
			m_genreType(type),
			m_name(name),
			m_filter(std::move(filter)),
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
				if (m_order >= 10) {
					return s3d::TextureAsset(U"genre_missing");
				} else if (m_order >= 9) {
					return s3d::TextureAsset(U"genre_level_star6");
				} else if (m_order >= 8) {
					return s3d::TextureAsset(U"genre_level_star5");
				} else if (m_order >= 7) {
					return s3d::TextureAsset(U"genre_level_star4");
				} else if (m_order >= 6) {
					return s3d::TextureAsset(U"genre_level_star3");
				} else if (m_order >= 5) {
					return s3d::TextureAsset(U"genre_level_star2");
				} else if(m_order >= 4) {
					return s3d::TextureAsset(U"genre_level_star1");
				} else if (m_order >= 3) {
					return s3d::TextureAsset(U"genre_level_aste3");
				} else if (m_order >= 2) {
					return s3d::TextureAsset(U"genre_level_aste2");
				} else {
					return s3d::TextureAsset(U"genre_level_aste1");
				}
			case GenreType::Favorite:
				return s3d::TextureAsset(U"genre_favorite");
			case GenreType::Custom:
				return s3d::TextureAsset(U"genre_custom_folder");
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
		const GenreFilter& getFilter() const
		{
			return m_filter;
		}
	private:
		s3d::String m_name;
		GenreFilter m_filter;
		GenreType m_genreType;  // ソート用
		s3d::int32 m_order;     // レベル用
	};

	GenreData GenreData::CreateAll()
	{
		return std::make_shared<GenreHandle>(
			GenreType::All, 
			U"ALL", 
			GenreFilter([]([[maybe_unused]] const MusicData& music)->bool {return true; })
		);
	}

	GenreData GenreData::CreateFavorite()
	{
		return std::make_shared<GenreHandle>(
			GenreType::Favorite, U"お気に入り",
			GenreFilter([](const MusicData& music)->bool {return music.isFavorite(); })
		);
	}

	GenreData GenreData::CreateFolder(const s3d::String& folderName)
	{
		return std::make_shared<GenreHandle>(
			GenreType::Folder, 
			folderName, 
			GenreFilter([folderName](const MusicData& music)->bool {return music.getGenreName() == folderName; })
		);
	}

	GenreData GenreData::CreateLv(s3d::int32 lv)
	{
		return std::make_shared<GenreHandle>(
			GenreType::Lv, 
			Format(U"LEVEL:", lv), 
			GenreFilter([lv](const NotesData& notes) {return notes.getLevel() == lv && notes.getStarLv() == StarLv::None; }, GenreFilterEvalMode::Any),
			lv
		);
	}

	GenreData GenreData::CreateStarLv(StarLv starLv)
	{
		return std::make_shared<GenreHandle>(
			GenreType::StarLv, 
			Format(U"LEVEL:", ToStr(starLv)),
			GenreFilter([starLv](const NotesData& notes) {return notes.getStarLv() == starLv; }, GenreFilterEvalMode::Any),
			static_cast<int32>(starLv)
		);
	}

	GenreData GenreData::CreateCustom(const CTCFReader& ctcf)
	{
		String title = ctcf.getTitle().value_or(U"CustomFolder");
		return std::make_shared<GenreHandle>(
			GenreType::Custom, 
			title,
			GenreFilter([ctcf](const NotesData& notes)->bool {return ctcf.expression(notes); }, ctcf.getEvalMode()),
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
	const GenreFilter& GenreData::getFilter() const
	{
		return m_handle->getFilter();
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
