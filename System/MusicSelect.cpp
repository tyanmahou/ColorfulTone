#include "MusicSelect.hpp"
#include"Fade.h"
#include"PlayKey.h"
#include"SceneInfo.h"
#include"GenreManager.h"

namespace
{
	MusicSelect::SelectMusicsInfo g_selectInfo;

	using SortMode = MusicSelect::SortMode;
	//　ソートの切り替え用
	SortMode NextMode(SortMode mode)
	{
		switch (mode)
		{
		case SortMode::FileName:return SortMode::MusicName;
		case SortMode::MusicName:return SortMode::ArtistName;
		case SortMode::ArtistName:return SortMode::LastUpdateAt;
		case SortMode::LastUpdateAt:return SortMode::FileName;
		}
		return MusicSelect::SortMode::Default;
	}
	// ソート関数の取得
	std::function<bool(const MusicData& l, const MusicData& r)> SortFunc(SortMode mode)
	{
		switch (mode)
		{
		case SortMode::FileName:return [](const MusicData& l, const MusicData& r)
		{
			return 	l.getIndex() < r.getIndex();
		};
		case SortMode::MusicName:return  [](const MusicData& l, const MusicData& r)
		{
			return 	l.getMusicName() < r.getMusicName();
		};
		case SortMode::ArtistName:return  [](const MusicData& l, const MusicData& r)
		{
			return 	l.getArtistName() < r.getArtistName();
		};
		case SortMode::LastUpdateAt:return  [](const MusicData& l, const MusicData& r)
		{
			return 	l.getLastUpdateAt() > r.getLastUpdateAt();
		};
		}
		return SortFunc(SortMode::Default);
	}

	// 楽曲リストソート
	void SortMusics(Array<MusicData>& musics)
	{
		std::sort(musics.begin(), musics.end(), SortFunc(g_selectInfo.sortMode));
	}

	// 楽曲リスト初期化
	void InitMusics(Array<MusicData>& musics)
	{
		musics = Game::Instance()->m_musics;
		Erase_if(musics, GenreManager::m_refiners[g_selectInfo.genre].m_refiner);
		::SortMusics(musics);
	}
}

MusicSelect::MusicSelect():
	m_view(this)
{}

MusicSelect::~MusicSelect()
{}

void MusicSelect::init()
{
	::InitMusics(m_musics);
}

void MusicSelect::update()
{
	const uint32 musicSize = m_musics.size();

	if (PlayKey::Left().clicked)
	{
		++g_selectInfo.music;
		m_shaderTimer = 0;
	}
	if (PlayKey::Right().clicked)
	{
		g_selectInfo.music += musicSize;
		--g_selectInfo.music;
		m_shaderTimer = 0;
	}
	m_shaderTimer += 0.025;
	g_selectInfo.music = musicSize ? g_selectInfo.music%musicSize : 0;
	//戻る
	if (PlayKey::BigBack().clicked)
	{
		SoundManager::SE::Play(L"cancel");
		changeScene(L"title", 1000, true);
	}
}

void MusicSelect::draw() const
{
	const int32 timer = System::FrameCount();
	m_view.draw();

	// シーン情報
	if (timer % 400 <= 200)
	{
		SceneInfo::Draw(L"Enter:決定　BackSpace:絞り込み,戻る　F2:ソート　Esc:タイトル戻る ");
	} else
	{
		SceneInfo::Draw(L"Shift:表示モード切替　F1:オート　Ctrl+↑↓:ハイスピード変更");
	}
}

void MusicSelect::drawFadeIn(double t) const
{
	FadeIn(Fade::FlipPage, t, [this]() {this->draw(); });
}

void MusicSelect::drawFadeOut(double t) const
{
	this->draw();
}

MusicSelect::SelectMusicsInfo MusicSelect::GetSelectInfo()
{
	return g_selectInfo;
}
