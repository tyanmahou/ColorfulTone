#include "MusicSelect.hpp"
#include"Fade.h"
#include"PlayKey.h"
#include"SceneInfo.h"
#include"GenreManager.h"
#include"Util.h"
#include"AutoPlayManager.h"

namespace
{
	MusicSelect::SelectMusicsInfo g_selectInfo;

	using SortMode = MusicSelect::SortMode;
	using Action = MusicSelect::Action;
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
	void RefineMusics(Array<MusicData>& musics)
	{
		Erase_if(musics, GenreManager::GetRefiner(g_selectInfo.genre));
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
		::RefineMusics(musics);
		::SortMusics(musics);

		size_t musicSize = musics.size();
		if (musicSize) {
			g_selectInfo.music %= musicSize;
		}
		else {
			g_selectInfo.music = 0;
		}
	}

	uint32& GetSelectTarget(Action action)
	{
		switch (action)
		{
		case Action::GenreSelect: return g_selectInfo.genre;
		case Action::MusicSelect: return g_selectInfo.music;
		case Action::LevelSelect: return g_selectInfo.level;
		default:
			break;
		}
		return g_selectInfo.music;
	}
	size_t GetTargetSize(Action action, const Array<MusicData>& musics)
	{
		switch (action)
		{
		case Action::GenreSelect: return GenreManager::Size();
		case Action::MusicSelect: return musics.size();
		case Action::LevelSelect: return musics[g_selectInfo.music].getNotesData().size();
		default:
			break;
		}
		return 0;
	}

	int MoveSelect()
	{
		if (util::AccelPressed(PlayKey::Down()))
		{
			return -1;
		}
		if (util::AccelPressed(PlayKey::Up()))
		{
			return 1;
		}
		return 0;
	}
}

MusicSelect::MusicSelect() :
	m_view(this)
{
	SoundAsset(L"title").stop(1s);
}

MusicSelect::~MusicSelect()
{}

void MusicSelect::init()
{
	::InitMusics(m_musics);
}

void MusicSelect::update()
{
	// 選択するターゲットの参照
	auto &target = ::GetSelectTarget(m_action);
	size_t size = ::GetTargetSize(m_action, m_musics);

	// ハイスピ変更
	bool isHighSpeedUpdate = m_highSpeedDemo.update(m_data->m_scrollRate);
	int m_moveSelect = isHighSpeedUpdate ? 0 : ::MoveSelect();
	if (m_moveSelect)
	{
		if (m_moveSelect < 0)
		{
			++target;
		}
		else
		{
			target += size;
			--target;
		}
		if (m_action == Action::MusicSelect)
		{
			m_shaderTimer = 0;
		}
		SoundManager::SE::Play(L"select");
	}
	target = size ? target % size : 0;

	m_shaderTimer += 0.025;

	// 決定ボタン
	if (PlayKey::Start().clicked && size)
	{
		if (m_action == Action::GenreSelect)
		{
			::InitMusics(m_musics);
			m_action = Action::MusicSelect;
			SoundManager::SE::Play(L"desisionSmall");
		}
		else if (m_action == Action::MusicSelect)
		{
			m_action = Action::LevelSelect;
			SoundManager::SE::Play(L"desisionSmall");
		}
		else if (m_action == Action::LevelSelect)
		{
			m_data->m_nowMusics = m_musics[g_selectInfo.music];
			m_data->m_selectMusic = m_data->m_nowMusics.getIndex();
			m_data->m_selectLevel = g_selectInfo.level;
			changeScene(L"main", 3000);
			SoundManager::SE::Play(L"desisionLarge");
		}
	}
	// キャンセルボタン
	if (PlayKey::SmallBack().clicked)
	{
		if (m_action == Action::MusicSelect)
		{
			m_action = Action::GenreSelect;
			SoundManager::SE::Play(L"cancel");
		}
		else if (m_action == Action::LevelSelect)
		{
			m_action = Action::MusicSelect;
			SoundManager::SE::Play(L"cancel");
		}
	}
	//プレイモード
	if (Input::KeyF1.clicked)
	{
		AutoPlayManager::Instance()->m_autoPlay = !(AutoPlayManager::Instance()->m_autoPlay);
		SoundManager::SE::Play(L"desisionSmall");
	}
	//ソート
	if (Input::KeyF2.clicked)
	{
		uint32 index = m_musics[g_selectInfo.music].getIndex();
		g_selectInfo.sortMode = ::NextMode(g_selectInfo.sortMode);
		::SortMusics(m_musics);

		for (uint32 i = 0; i < m_musics.size(); ++i)
		{
			if (index == m_musics[i].getIndex())
			{
				g_selectInfo.music = i;
				break;
			}
		}
		SoundManager::SE::Play(L"desisionSmall");
	}
	//戻る
	if (PlayKey::BigBack().clicked)
	{
		changeScene(L"title", 1000, true);
		SoundManager::SE::Play(L"cancel");
	}
}

namespace
{
	// シーン情報のメッセージを取得
	String GetSceneInfoMsg()
	{
		const int32 timer = System::FrameCount();

		if (timer % 400 <= 200)
		{
			return L"Enter:決定　BackSpace:絞り込み,戻る　F2:ソート　Esc:タイトル戻る";
		}

		return L"Shift:表示モード切替　F1:オート　Ctrl+↑↓:ハイスピード変更";
	}
}

void MusicSelect::draw() const
{
	const int32 timer = System::FrameCount();
	m_view.draw();
	// シーン情報
	SceneInfo::Draw(::GetSceneInfoMsg());

	if (AutoPlayManager::Instance()->m_autoPlay)
	{
		PutText(L"AutoPlay").at(Window::Center().x, 40);
	}
}

void MusicSelect::drawFadeIn(double t) const
{
	FadeIn(Fade::FlipPage, t, [this]() {this->draw(); });
}

void MusicSelect::drawFadeOut(double t) const
{
	this->draw();
	FadeOut(static_cast<void(*)(double, const Color&)>(Fade::DrawCanvas), t, Palette::White);
	m_musics[g_selectInfo.music].getTexture().resize(300, 300).drawAt(400, 300, ColorF(1, t*t));
}

MusicSelect::SelectMusicsInfo MusicSelect::GetSelectInfo()
{
	return g_selectInfo;
}
