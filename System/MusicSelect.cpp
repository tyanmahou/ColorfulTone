#include "MusicSelect.hpp"
#include "Useful.hpp"
#include"GenreManager.h"
#include"AutoPlayManager.h"
#include "HighSpeedDemo.h"
#include"Audition.hpp"

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
		util::Erase_not_if(musics, GenreManager::GetRefiner(g_selectInfo.genre));
	}

	// 楽曲リストソート
	void SortMusics(Array<MusicData>& musics)
	{
		std::sort(musics.begin(), musics.end(), SortFunc(g_selectInfo.sortMode));
	}

	// 楽曲リスト初期化
	void InitMusics(Array<MusicData>& musics)
	{
		musics = Game::Musics();
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

class MusicSelect::Model
{
	std::shared_ptr<GameData> m_data;
	Action m_action = Action::MusicSelect;
	Action m_prevAction = Action::MusicSelect;
	int m_moveSelect = 0;
	int m_prevSelect = -1;

	Array<MusicData> m_musics;
	Audition m_audition;
	HighSpeedDemo m_highSpeedDemo;

	bool m_isSelectedNotes = false;
public:
	void setData(std::shared_ptr<GameData> data)
	{
		m_data = data;
	}

	void init()
	{
		::InitMusics(m_musics);
		if (m_musics.size())
		{
			m_audition.autoPlayAndStop(m_musics[g_selectInfo.music]);
		}
	}

	void update()
	{
		m_prevAction = m_action;
		// 選択するターゲットの参照
		auto &target = ::GetSelectTarget(m_action);
		m_prevSelect = g_selectInfo.music;
		size_t size = ::GetTargetSize(m_action, m_musics);

		// ハイスピ変更
		bool isHighSpeedUpdate = m_highSpeedDemo.update(m_data->m_scrollRate);
		m_moveSelect = isHighSpeedUpdate ? 0 : ::MoveSelect();
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

			SoundManager::SE::Play(L"select");
		}
		target = size ? target % size : 0;

		// 決定ボタン
		if (PlayKey::Start().clicked && size)
		{
			if (m_action == Action::GenreSelect)
			{
				::InitMusics(m_musics);
				m_prevSelect = -1; //曲の変更をトリガー

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
				m_isSelectedNotes = true;
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
			AutoPlayManager::ChangePlayMode();
			SoundManager::SE::Play(L"desisionSmall");
		}
		//情報切り替え
		if (Input::KeyShift.clicked)
		{
			SoundManager::SE::Play(L"desisionSmall");
			g_selectInfo.notesInfo = g_selectInfo.notesInfo == AllNotesInfo::Level
				? AllNotesInfo::ClearRank : AllNotesInfo::Level;
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
		// 再度indexの調整
		{
			auto &target = ::GetSelectTarget(m_action);
			size_t size = ::GetTargetSize(m_action, m_musics);
			target = size ? target % size : 0;
		}
		// 試聴
		if (m_musics.size() && !(m_action == Action::MusicSelect && (PlayKey::Up().pressed || PlayKey::Down().pressed)))
		{
			m_audition.autoPlayAndStop(m_musics[g_selectInfo.music]);
		}
	}

	void finally()
	{
		m_audition.stop();
	}

	bool onChangeSelectMusic()
	{
		return m_prevSelect != g_selectInfo.music;
	}

	bool onChangeAction()
	{
		return m_action != m_prevAction;
	}
	const NotesData& getSelectNotes()const
	{
		return m_musics[g_selectInfo.music][g_selectInfo.level];
	}

	const Array<MusicData>& getMusics()const
	{
		return m_musics;
	}

	Action getAction()const
	{
		return m_action;
	}

	// previous , current
	std::pair<Action, Action> getChangeAction()const
	{
		return { m_prevAction ,m_action };
	}

	int getMoveSelect()const
	{
		return m_moveSelect;
	}

	const HighSpeedDemo& getHighSpeedDemo()const
	{
		return m_highSpeedDemo;
	}
	bool isSelectedNotes()
	{
		return m_isSelectedNotes;
	}
};
MusicSelect::MusicSelect() :
	m_pModel(std::make_shared<Model>()),
	m_view(this)
{}

void MusicSelect::init()
{
	m_pModel->setData(m_data);
	m_pModel->init();
}

void MusicSelect::finally()
{
	m_pModel->finally();
	if (m_data->m_toScene == SceneName::Main)
	{
		m_data->m_nowNotes = m_pModel->getSelectNotes();
	}
}

void MusicSelect::update()
{
	m_pModel->update();

	if (m_pModel->isSelectedNotes())
	{
		changeScene(SceneName::Main, 2000, false);
		SoundManager::SE::Play(L"desisionLarge");
	}
	else if (PlayKey::BigBack().clicked)
	{
		//戻る
		changeScene(SceneName::Title, 1000);
		SoundManager::SE::Play(L"cancel");
	}

	m_view.update();
	if (m_pModel->onChangeSelectMusic())
	{
		m_view.resetShaderTimer();
	}
	if (m_pModel->onChangeAction())
	{
		m_view.onChangeAction();
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
	m_view.draw();
	// シーン情報
	SceneInfo::Draw(::GetSceneInfoMsg());

	if (AutoPlayManager::IsAutoPlay())
	{
		PutText(L"AutoPlay").at(Window::Center().x, 40);
	}
}

void MusicSelect::drawFadeIn(double t) const
{
	if (m_data->m_fromScene == SceneName::Main)
	{
		this->draw();
		FadeIn(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
	}
	else
	{
		FadeIn(Fade::FlipPage, t, [this]() {this->draw(); }, true);
	}
}

void MusicSelect::drawFadeOut(double t) const
{
	if (m_data->m_toScene == SceneName::Main)
	{
		this->draw();
		FadeOut(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
		const double size = EaseOut(300.0, 350.0, Easing::Cubic, t);
		m_data->m_nowNotes
			.getMusic()
			->getTexture()
			.resize(size, size)
			.drawAt(400, 300, ColorF(1, t*t));
	}
	else
	{
		this->draw();
	}
}

MusicSelect::SelectMusicsInfo MusicSelect::GetSelectInfo()
{
	return g_selectInfo;
}

const Array<MusicData>& MusicSelect::getMusics() const
{
	return m_pModel->getMusics();
}

Action MusicSelect::getAction() const
{
	return m_pModel->getAction();
}

std::pair<Action, Action> MusicSelect::getChangeAction() const
{
	return m_pModel->getChangeAction();
}

int MusicSelect::getMoveSelect() const
{
	return m_pModel->getMoveSelect();
}

const HighSpeedDemo & MusicSelect::getHighSpeedDemo() const
{
	return m_pModel->getHighSpeedDemo();
}
