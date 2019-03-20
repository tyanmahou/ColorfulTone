#include"TitleScene.h"
#include "Useful.hpp"

#include"FontKinetic.h"
#include"Setting.hpp"
#include"SharedDraw.hpp"

//--------------------------------------------------------------------------------
//関数：コンストラクタ
//--------------------------------------------------------------------------------

TitleScene::Mode TitleScene::m_mode = TitleScene::Mode::GameStart;


TitleScene::TitleScene() :
	m_timer(0),
	m_font(10, L"Straight", FontStyle::Outline)
{
	m_font.changeOutlineStyle(TextOutlineStyle(Palette::White, Palette::White, 2));
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
		{Mode::Tutorial, {SceneName::Tutorial, 2000, false}},
		{Mode::Reload, {SceneName::Load, 1000, true}},
	};

	if (m_mode == Mode::Exit)
	{
		System::Exit();
		return;
	}

	if (m_mode == Mode::Download)
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
	m_timer++;
	if (PlayKey::Up().clicked)
	{
		SoundManager::SE::Play(L"select");
		--m_mode;
	}
	else 	if (PlayKey::Down().clicked)
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
	TextureAsset(L"canvasBg").draw();
	static SharedDraw::DrawBGLight lights;
	lights.update();
	lights.draw();
	const double timer = static_cast<double>(System::FrameCount());
	static RenderTexture tex(700, 500);
	{
		auto render = util::RenderTextureUtil(tex);
		auto t2d = render.getTransformer2D();
		TextureAsset(L"titleBg1").uv(timer / 15000.0, 1.0, 0.75, 1.0).draw(0, 0);
		TextureAsset(L"titleBg2").uv(timer / 6000.0, 1.0, 0.75, 1.0).draw(0, 0);
		TextureAsset(L"titleBg3").uv(timer / 2000.0, 1.0, 0.75, 1.0).draw(0, 0);
	}
	Graphics2D::SetBlendState(BlendState::Multiplicative);
	tex.draw(50, 60);
	Graphics2D::SetBlendState(BlendState::Default);


	TextureAsset(L"logo").scale(0.8).drawAt(400, 150);

	static Array<std::pair<String, String>> name = {
		{ L"iconPlay",L"FREE PLAY"},
		{ L"iconPlayCourse",L"COURSE PLAY" },
		{ L"iconConfig",L"CONFIG"},
		{ L"iconReset",L"RELOAD"},
		{ L"iconTutorial",L"TUTORIAL" },
		{ L"iconAccess",L"ACCESS" },
		{ L"iconShutdown",L"EXIT",}
	};

	constexpr double x = 400;


	constexpr double y = 350;
	constexpr int off = 40;

	TextureAsset(L"modeBack").drawAt(x, y + 40 * 2, ColorF(0.8, 1, 1, 0.9 + 0.1*Sin(static_cast<double>(m_timer) / 30.0)));

	for (unsigned int i = 0; i < 5; ++i)
	{
		const unsigned index = (static_cast<unsigned int>(m_mode) + i + 5) % 7;
		const auto size = m_font(name[index].second).region().size;
		m_font(name[index].second).drawKinetic(x - size.x / 2.0, y + off * i - size.y / 2.0, FontKinetic::DeleteSpace, ColorF(0, i == 2 ? 1 : 0.5 - 0.1*Abs<int>(2 - i)));

		TextureAsset(name[index].first).scale(0.15 + (i == 2 ? 0.012*Sin(static_cast<double>(m_timer) / 20.0) : 0)).drawAt(x - 150, y + off * i, ColorF(i == 2, 1 - 0.3*Abs<int>(2 - i)));
	}


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