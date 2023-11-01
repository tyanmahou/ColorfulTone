#include <core/Anime/ApAnime/ApAnime.hpp>
#include <core/Data/Score/Score.hpp>
#include <utils/Flipbook/Flipbook.hpp>
#include <Siv3D.hpp>

namespace ct
{
	class ApAnime::Impl
	{
		enum class Type
		{
			FC,
			AP,
		};
	private:
		bool m_isAnimeStart;
		Type m_type;
		Flipbook m_fcAnime;
		Flipbook m_apAnime;
	public:
		Impl() :
			m_isAnimeStart(false),
			m_type(Type::FC),
			m_fcAnime(TextureAsset(U"fcAnime"), { 8,5 }, false, 20_fps),
			m_apAnime(TextureAsset(U"apAnime"), { 8,5 }, false, 20_fps)
		{
			TextureAsset::Release(U"fcAnime");
			TextureAsset::Release(U"apAnime");
		}
		~Impl() = default;
		bool isStart() const
		{
			return m_isAnimeStart;
		}
		bool isEnd() const
		{
			return m_apAnime.isEnd() || m_fcAnime.isEnd();
		}
		void play(const Score& score)
		{
			m_isAnimeStart = true;
			if (score.m_maxCombo == score.m_judgeCount[Score::Perfect]) {
				m_type = Type::AP;
			} else {
				m_type = Type::FC;
			}
		}
		void update()
		{
			if (m_type == Type::AP) {
				m_apAnime.update();
			} else {
				m_fcAnime.update();
			}
		}
		void draw()const
		{
			if (m_type == Type::AP) {
				m_apAnime.scale(2.0).drawAt({ 400,300 });
				TextureAsset(U"apLogo").drawAt(400, 300);
			} else {
				m_fcAnime.scale(2.0).drawAt({ 400,300 });
				TextureAsset(U"fcLogo").drawAt(400, 300);
			}
		}
	};

	ApAnime::ApAnime() :
		m_pImpl(std::make_unique<Impl>())
	{}


	ApAnime::~ApAnime()
	{}

	bool ApAnime::isStart() const
	{
		return m_pImpl->isStart();
	}

	bool ApAnime::isEnd() const
	{
		return m_pImpl->isEnd();
	}

	void ApAnime::play(const Score& score) const
	{
		m_pImpl->play(score);
	}

	void ApAnime::update() const
	{
		m_pImpl->update();
	}

	void ApAnime::draw() const
	{
		m_pImpl->draw();
	}
}
