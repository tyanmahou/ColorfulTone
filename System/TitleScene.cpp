#include"TitleScene.h"
#include "Useful.hpp"

#include"FontKinetic.h"
#include"Setting.hpp"
#include"SharedLogic.hpp"
//--------------------------------------------------------------------------------
//関数：コンストラクタ
//--------------------------------------------------------------------------------

TitleScene::Mode TitleScene::m_mode = TitleScene::Mode::GameStart;


TitleScene::TitleScene():
	m_view(this)
{
	SoundAsset::PreloadByTag(L"System");
}

//--------------------------------------------------------------------------------
//関数：演算子オーバーロード
//--------------------------------------------------------------------------------

TitleScene::Mode operator ++(TitleScene::Mode& mode)
{
	mode = (mode == TitleScene::Mode::Exit) ? TitleScene::Mode::GameStart : TitleScene::Mode(static_cast<int>(mode) + 1);
	return mode;
}
TitleScene::Mode operator --(TitleScene::Mode& mode)
{
	mode = (mode == TitleScene::Mode::GameStart) ? TitleScene::Mode::Exit : TitleScene::Mode(static_cast<int>(mode) - 1);
	return mode;
}
//--------------------------------------------------------------------------------
//関数：update
//--------------------------------------------------------------------------------

namespace
{
	void AccessHomePage()
	{
		const FilePath url = Setting::HOMEPAGE_URL;

		if (Internet::IsConnected()) // インターネット接続をチェック
		{
			Internet::LaunchWebBrowser(url);
		}
		else
		{
			MessageBox::Show(L"インターネットに接続できませんでした。");
		}
	}
}

void TitleScene::onEnterMode()
{
	SoundManager::SE::Play(L"desisionLarge");
	using ChangeSceneParam = struct
	{
		String name;
		int timeMillisec;
		bool crossFade;
	};
	static const std::unordered_map<Mode, ChangeSceneParam> sceneParams{
		{Mode::GameStart, {SceneName::Select, 1000, true}},
		{Mode::Course,{SceneName::CourseSelect, 1000, true}},
		{Mode::KeyConfig, {SceneName::Config, 1000, true}},
		{Mode::Download, {SceneName::Download, 1000, true}},
		{Mode::Reload, {SceneName::Load, 1000, true}},
		{Mode::Tutorial, {SceneName::Tutorial, 2000, false}},
	};

	if (m_mode == Mode::Exit)
	{
		System::Exit();
		return;
	}

	if (m_mode == Mode::Access)
	{
		if (MessageBox::Show(L"インターネットに接続しホームページにアクセスします。", MessageBoxStyle::OkCancel) == MessageBoxCommand::Ok)
		{
			::AccessHomePage();
		}
		return;
	}
	const auto& param = sceneParams.at(m_mode);
	changeScene(param.name, param.timeMillisec, param.crossFade);
}

void TitleScene::init()
{
	if (!SoundAsset(L"title").isPlaying())
	{
		SoundManager::BGM::Play(L"title", 1s);
	}
	// ロードから来た場合はスタートに戻す
	if (m_data->m_fromScene == SceneName::Load)
	{
		m_mode = Mode::GameStart;
	}
}

void TitleScene::finally()
{
	if (m_data->m_toScene == SceneName::Select || m_data->m_toScene == SceneName::Tutorial)
	{
		SoundAsset(L"title").stop(1s);
	}
}

void TitleScene::update()
{
	int move = SharedLogic::MoveSelect();

	if (move > 0)
	{
		SoundManager::SE::Play(L"select");
		--m_mode;
	}
	else if (move < 0)
	{
		SoundManager::SE::Play(L"select");
		++m_mode;
	}
	if (PlayKey::Start().clicked)
	{
		this->onEnterMode();
	}

	if (PlayKey::BigBack().clicked)
	{
		SoundManager::SE::Play(L"select");
		m_mode = Mode::Exit;
	}

}
//--------------------------------------------------------------------------------
//関数：draw
//--------------------------------------------------------------------------------
void TitleScene::draw()const
{
	m_view.draw();
	SceneInfo::Draw(L"Press Enter");
	FontAsset(L"info")(Game::Version).draw(10, 600 - 40 + 10);
}

//--------------------------------------------------------------------------------
//関数：drawFadeIn
//--------------------------------------------------------------------------------
void TitleScene::drawFadeIn(double t) const
{
	if(m_data->m_fromScene == SceneName::Tutorial)
	{ 
		this->draw();
		FadeIn(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
	}
	else if(m_data->m_fromScene == SceneName::Load)
	{
		FadeIn(Fade::FlipPage, t, [this]() {draw(); }, true);
	}
	else
	{
		FadeOut(Fade::FlipPage, t, [this]() {draw(); }, false);
	}
}

//--------------------------------------------------------------------------------
//関数：drawFadeOut
//--------------------------------------------------------------------------------
void TitleScene::drawFadeOut(double t) const
{
	if (this->m_data->m_toScene == SceneName::Tutorial)
	{
		this->draw();
		FadeOut(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
	}
	else
	{
		this->draw();
	}
}

TitleScene::Mode TitleScene::GetMode()
{
	return m_mode;
}
