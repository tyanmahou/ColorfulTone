#include "APAnime.hpp"
#include"SerialImage.h"
#include"Score.h"
#include<Siv3D.hpp>

class APAnime::Impl
{
	enum class Type
	{
		FC,
		AP,
	};
private:
	bool m_isAnimeStart;
	Type m_type;
	SerialImage m_fcAnime;
	SerialImage m_apAnime;
public:
	Impl() :
		m_isAnimeStart(false),
		m_type(Type::FC),
		m_fcAnime(TextureAsset(L"fcAnime"), { 8,5 }, 2, false),
		m_apAnime(TextureAsset(L"apAnime"), { 8,5 }, 2, false)
	{
		TextureAsset::Release(L"fcAnime");
		TextureAsset::Release(L"apAnime");
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
		if (score.m_maxCombo == score.m_judgeCount[Score::Perfect])
		{
			m_type = Type::AP;
		}
		else
		{
			m_type = Type::FC;
		}
	}
	void update()
	{
		if (m_type == Type::AP)
		{
			m_apAnime.update();
		}
		else
		{
			m_fcAnime.update();
		}
	}
	void draw()const
	{
		if (m_type == Type::AP)
		{
			m_apAnime.scale(2.0).drawAt({ 400,300 });
			TextureAsset(L"apLogo").drawAt(400, 300);
		}
		else
		{
			m_fcAnime.scale(2.0).drawAt({ 400,300 });
			TextureAsset(L"fcLogo").drawAt(400, 300);
		}
	}
};

APAnime::APAnime():
	m_pImpl(std::make_shared<Impl>())
{}


APAnime::~APAnime()
{}

bool APAnime::isStart() const
{
	return m_pImpl->isStart();
}

bool APAnime::isEnd() const
{
	return m_pImpl->isEnd();
}

void APAnime::play(const Score & score) const
{
	m_pImpl->play(score);
}

void APAnime::update() const
{
	m_pImpl->update();
}

void APAnime::draw() const
{
	m_pImpl->draw();
}
