#include"Game.h"

#include"CourseData.h"
#include"GameConfig.h"

#include"SoundManager.h"
#include"ISceneBase.hpp"
#include"TitleScene.h"
#include"FileLoad.h"
#include"Tutorial.h"
#include"KeyConfigScene.h"
#include"MusicSelect.hpp"
#include"MainScene.h"
#include"ResultScene.hpp"
#include"VideoAsset.h"
#include"CourseSelect.hpp"
#include"CourseScene.hpp"
#include"DownloadScene.hpp"
//--------------------------------------------------------------------------------
//関数：LoadTexture
//--------------------------------------------------------------------------------
//概要：画像の登録
//--------------------------------------------------------------------------------

namespace
{
	void LoadTexture()
	{
		// ロード
		TextureAsset::Register(L"canvasBg", L"Resource/Img/Load/bg.png");

		//タイトル
		TextureAsset::Register(L"titleBgs", L"Resource/Img/Title/title_bgs.png");
		TextureAsset::Register(L"logo", L"Resource/Img/Title/logo.png");
		TextureAsset::Register(L"modeBack", L"Resource/Img/Title/modeBack.png");
		TextureAsset::Register(L"titleIcons", L"Resource/Img/Title/title_icons.png");

		//キーコン
		TextureAsset::Register(L"keyconBG2", L"Resource/Img/KeyConfig/keyconBG2.png");

		//選曲
		TextureAsset::Register(L"sticky", L"Resource/Img/MusicSelect/sticky.png");
		TextureAsset::Register(L"sticky_red", L"Resource/Img/MusicSelect/sticky_red.png");
		TextureAsset::Register(L"line", L"Resource/Img/MusicSelect/line.png");
		TextureAsset::Register(L"memo", L"Resource/Img/MusicSelect/memo.png");
		TextureAsset::Register(L"memoCourse", L"Resource/Img/MusicSelect/memo_course.png");
		TextureAsset::Register(L"memoDL", L"Resource/Img/MusicSelect/memo_dl.png");
		TextureAsset::Register(L"favorite", L"Resource/Img/MusicSelect/favorite.png");

		TextureAsset::Register(L"levelbg", L"Resource/Img/MusicSelect/select.png");
		TextureAsset::Register(L"levelMask", L"Resource/Img/MusicSelect/levelmask.png");

		//ジャンルアイコン
		TextureAsset::Register(L"genre_level1", L"Resource/Img/MusicSelect/icon/level1.png");
		TextureAsset::Register(L"genre_level2", L"Resource/Img/MusicSelect/icon/level2.png");
		TextureAsset::Register(L"genre_level3", L"Resource/Img/MusicSelect/icon/level3.png");
		TextureAsset::Register(L"genre_level4", L"Resource/Img/MusicSelect/icon/level4.png");
		TextureAsset::Register(L"genre_level5", L"Resource/Img/MusicSelect/icon/level5.png");
		TextureAsset::Register(L"genre_file", L"Resource/Img/MusicSelect/icon/file.png");
		TextureAsset::Register(L"genre_all", L"Resource/Img/MusicSelect/icon/red.png");
		TextureAsset::Register(L"genre_favorite", L"Resource/Img/MusicSelect/icon/favorite.png");

		//リザルトシーン
		TextureAsset::Register(L"AAA", L"Resource/Img/Result/rank/AAA.png");
		TextureAsset::Register(L"AA", L"Resource/Img/Result/rank/AA.png");
		TextureAsset::Register(L"A", L"Resource/Img/Result/rank/A.png");
		TextureAsset::Register(L"B", L"Resource/Img/Result/rank/B.png");
		TextureAsset::Register(L"C", L"Resource/Img/Result/rank/C.png");
		TextureAsset::Register(L"D", L"Resource/Img/Result/rank/D.png");
		TextureAsset::Register(L"E", L"Resource/Img/Result/rank/E.png");
		TextureAsset::Register(L"F", L"Resource/Img/Result/rank/F.png");
		TextureAsset::Register(L"iconAP", L"Resource/Img/Result/rank/allperfects.png");
		TextureAsset::Register(L"iconFC", L"Resource/Img/Result/rank/fullcombos.png");
		TextureAsset::Register(L"iconClear", L"Resource/Img/Result/rank/clears.png");

		TextureAsset::Register(L"newRecord", L"Resource/Img/Result/newrecord.png");

		//メイン
		//portrait

		TextureAsset::Register(L"tapEffect2", L"Resource/Img/Main/tapeffect2.png");

		TextureAsset::Register(L"portrait_notes9", L"Resource/Img/Main/ノーツ/portrait/portrait_notes.png");
		TextureAsset::Register(L"portrait_notes1", L"Resource/Img/Main/ノーツ/portrait/portrait_notes1.png");
		TextureAsset::Register(L"portrait_notes2", L"Resource/Img/Main/ノーツ/portrait/portrait_notes2.png");
		TextureAsset::Register(L"portrait_notes3", L"Resource/Img/Main/ノーツ/portrait/portrait_notes3.png");
		TextureAsset::Register(L"portrait_notes4", L"Resource/Img/Main/ノーツ/portrait/portrait_notes4.png");
		TextureAsset::Register(L"portrait_notes5", L"Resource/Img/Main/ノーツ/portrait/portrait_notes5.png");
		TextureAsset::Register(L"portrait_notes6", L"Resource/Img/Main/ノーツ/portrait/portrait_notes6.png");
		TextureAsset::Register(L"portrait_notes7", L"Resource/Img/Main/ノーツ/portrait/portrait_notes7.png");
		TextureAsset::Register(L"portrait_notes10", L"Resource/Img/Main/ノーツ/portrait/portrait_notes10.png");

		TextureAsset::Register(L"mainbg", L"Resource/Img/Main/back.png");
		//隕石
		TextureAsset::Register(L"note_black", L"Resource/Img/Main/ノーツ/通常/kuro.png");
		TextureAsset::Register(L"note_red", L"Resource/Img/Main/ノーツ/通常/aka.png");
		TextureAsset::Register(L"note_blue", L"Resource/Img/Main/ノーツ/通常/ao.png");
		TextureAsset::Register(L"note_yellow", L"Resource/Img/Main/ノーツ/通常/ki.png");
		TextureAsset::Register(L"note_orange", L"Resource/Img/Main/ノーツ/通常/daidai.png");
		TextureAsset::Register(L"note_green", L"Resource/Img/Main/ノーツ/通常/midori.png");
		TextureAsset::Register(L"note_purple", L"Resource/Img/Main/ノーツ/通常/murasaki.png");
		TextureAsset::Register(L"note_white", L"Resource/Img/Main/ノーツ/通常/siro.png");
		TextureAsset::Register(L"note_rainbow", L"Resource/Img/Main/ノーツ/通常/rainbow.png");
		//彗星
		TextureAsset::Register(L"comet_red", L"Resource/Img/Main/ノーツ/ロング/aka.png");
		TextureAsset::Register(L"comet_blue", L"Resource/Img/Main/ノーツ/ロング/ao.png");
		TextureAsset::Register(L"comet_yellow", L"Resource/Img/Main/ノーツ/ロング/ki.png");
		TextureAsset::Register(L"comet_orange", L"Resource/Img/Main/ノーツ/ロング/daidai.png");
		TextureAsset::Register(L"comet_green", L"Resource/Img/Main/ノーツ/ロング/midori.png");
		TextureAsset::Register(L"comet_purple", L"Resource/Img/Main/ノーツ/ロング/murasaki.png");
		TextureAsset::Register(L"comet_black", L"Resource/Img/Main/ノーツ/ロング/kuro.png");
		TextureAsset::Register(L"comet_rainbow_head", L"Resource/Img/Main/ノーツ/ロング/rainbow_head.png");
		TextureAsset::Register(L"comet_rainbow_tail", L"Resource/Img/Main/ノーツ/ロング/rainbow_tail.png");

		//
		TextureAsset::Register(L"center_base", L"Resource/Img/Main/center/base.png");
		TextureAsset::Register(L"center_redlight", L"Resource/Img/Main/center/redlight.png");
		TextureAsset::Register(L"center_bluelight", L"Resource/Img/Main/center/bluelight.png");
		TextureAsset::Register(L"center_yellowlight", L"Resource/Img/Main/center/yellowlight.png");
		TextureAsset::Register(L"center_1", L"Resource/Img/Main/center/red.png");
		TextureAsset::Register(L"center_2", L"Resource/Img/Main/center/blue.png");
		TextureAsset::Register(L"center_3", L"Resource/Img/Main/center/yellow.png");
		TextureAsset::Register(L"center_4", L"Resource/Img/Main/center/green.png");
		TextureAsset::Register(L"center_5", L"Resource/Img/Main/center/orange.png");
		TextureAsset::Register(L"center_6", L"Resource/Img/Main/center/purple.png");
		TextureAsset::Register(L"center_7", L"Resource/Img/Main/center/white.png");

		//バー
		TextureAsset::Register(L"barBase", L"Resource/Img/Main/bar/base.png");
		TextureAsset::Register(L"bar1", L"Resource/Img/Main/bar/bar1.png");
		TextureAsset::Register(L"bar2", L"Resource/Img/Main/bar/bar2.png");
		TextureAsset::Register(L"streamPosBase", L"Resource/Img/Main/bar/streamPosBase.png");

		TextureAsset::Register(L"tapEffct", L"Resource/Img/Main/prasma.png");

		TextureAsset::Register(L"combo", L"Resource/Img/Main/combo.png");

		// main start
		TextureAsset::Register(L"start01", L"Resource/Img/Main/start/01.png");
		TextureAsset::Register(L"start02", L"Resource/Img/Main/start/02.png");
		TextureAsset::Register(L"start03", L"Resource/Img/Main/start/03.png");
		TextureAsset::Register(L"start04", L"Resource/Img/Main/start/04.png");
		TextureAsset::Register(L"start05", L"Resource/Img/Main/start/05.png");
		TextureAsset::Register(L"start06", L"Resource/Img/Main/start/06.png");
		TextureAsset::Register(L"start07", L"Resource/Img/Main/start/07.png");

		// resut
		TextureAsset::Register(L"memo2", L"Resource/Img/Result/memo2.png");

		TextureAsset::Register(L"fcAnime", L"Resource/Img/Result/fcAnime.png");
		TextureAsset::Register(L"apAnime", L"Resource/Img/Result/apAnime.png");
		TextureAsset::Register(L"fcLogo", L"Resource/Img/Result/fc.png");
		TextureAsset::Register(L"apLogo", L"Resource/Img/Result/ap.png");
		TextureAsset::Register(L"pass", L"Resource/Img/Result/pass.png");
		TextureAsset::Register(L"noPass", L"Resource/Img/Result/noPass.png");

		TextureAsset::PreloadAll();
	}
	//--------------------------------------------------------------------------------
	//関数：LoadSound
	//--------------------------------------------------------------------------------
	//概要：音の登録
	//--------------------------------------------------------------------------------

	void LoadSound()
	{

		// BGM
		SoundAsset::Register(L"title", L"Resource/Sound/BGM/title.mp3", SoundLoop(true), { L"System" });
		SoundAsset(L"title").setLoopBySec(true, 10.66s, 160.0s);

		SoundManager::BGM::Register(L"title");
		SoundAsset::Register(L"result", L"Resource/Sound/BGM/result.mp3", SoundLoop(true), { L"System" });
		SoundAsset(L"result").setLoopBySec(true, 6s, 90.0s);
		SoundManager::BGM::Register(L"result");

		// SE
		SoundAsset::Register(L"select", L"Resource/Sound/SE/select.mp3", { L"System" });
		SoundAsset::Register(L"desisionSmall", L"Resource/Sound/SE/decide.mp3", { L"System" });
		SoundAsset::Register(L"desisionLarge", L"Resource/Sound/SE/mekuru.mp3", { L"System" });
		SoundAsset::Register(L"desisionLarge2", L"Resource/Sound/SE/ironuri.mp3", { L"System" });
		SoundAsset::Register(L"cancel", L"Resource/Sound/SE/cancel.mp3", { L"System" });
	}
	void LoadFont()
	{
		FontManager::Register(L"Resource/Font/Straightfont.ttf");

		FontAsset::Register(L"judge", 8, L"Straight");
		FontAsset::Register(L"8o_s", 8, L"Straight", FontStyle::Outline);
		FontAsset(L"8o_s").changeOutlineStyle(TextOutlineStyle(Palette::Black, Palette::Black, 1));
		FontAsset::Register(L"info", 10, Typeface::Default, FontStyle::Bold);
		FontAsset::Register(L"combo", 10, L"Straight");
		FontAsset::Register(L"10o_s", 10, L"Straight", FontStyle::Outline);
		FontAsset(L"10o_s").changeOutlineStyle(TextOutlineStyle(Palette::White, Palette::White, 2));
		FontAsset::Register(L"bpm", 12);
		FontAsset::Register(L"level", 12, L"Straight", FontStyle::Outline);
		FontAsset(L"level").changeOutlineStyle(TextOutlineStyle(Palette::White, Palette::White, 2));
		FontAsset::Register(L"configSelect", 13);
		// ラベル
		FontAsset::Register(L"label", 15, L"Straight", FontStyle::Italic);
		FontAsset::Register(L"configTitle", 15, Typeface::Default, FontStyle::Italic);
		// 選曲画面などの選択
		FontAsset::Register(L"selectMusics", 16, Typeface::Bold);

		FontAsset::Register(L"20", 20);
		// リザルト
		FontAsset::Register(L"20b_s", 20, L"Straight", FontStyle::Bold);

	}

	void AssetLoad()
	{
		::LoadFont();
		::LoadTexture();
		::LoadSound();
	}
}

//--------------------------------------------------------------------------------
//関数：Init
//--------------------------------------------------------------------------------
//概要：ゲームの初期化
//--------------------------------------------------------------------------------

class Game::Impl
{
private:

	MyApp m_scene; //シーン

	void registerScene()
	{
		m_scene.add<FileLoad>(SceneName::Load);
		m_scene.add<TitleScene>(SceneName::Title);
		m_scene.add<ConfigScene>(SceneName::Config);
		m_scene.add<Tutorial>(SceneName::Tutorial);
		m_scene.add<MusicSelect>(SceneName::Select);
		m_scene.add<MainScene>(SceneName::Main);
		m_scene.add<ResultScene>(SceneName::Result);
		m_scene.add<CourseSelect>(SceneName::CourseSelect);
		m_scene.add<CourseScene>(SceneName::Course);
		m_scene.add<DownloadScene>(SceneName::Download);
	}
	void init()
	{
		this->registerScene();
		::AssetLoad();
	}
public:
	Array<MusicData> m_musics;
	Array<CourseData> m_courses;
	Array<TapSE> m_tapSEs;

	Effect m_effect;		//エフェクト
	GameConfig m_config;

	bool m_isMusicLoadEnd;

	Impl() :
		m_isMusicLoadEnd(false)
	{
		this->init();
	};

	bool updateAndDraw()
	{
		if (!m_scene.updateAndDraw())
		{
			return false;
		}
		return true;
	}
};

Game::Game():
	m_pImpl(std::make_shared<Impl>())
{}

bool Game::UpdateAndDraw()
{
	return Instance()->m_pImpl->updateAndDraw();
}

bool Game::SetLoadCompleted(bool completed)
{
	return Instance()->m_pImpl->m_isMusicLoadEnd = completed;
}

bool Game::IsLoadCompleted()
{
	return Instance()->m_pImpl->m_isMusicLoadEnd;
}

Array<MusicData>& Game::Musics()
{
	return Instance()->m_pImpl->m_musics;
}
Array<CourseData>& Game::Courses()
{
	return Instance()->m_pImpl->m_courses;
}
Array<TapSE>& Game::TapSEs()
{
	return Instance()->m_pImpl->m_tapSEs;
}
GameConfig& Game::Config()
{
	return Instance()->m_pImpl->m_config;
}
