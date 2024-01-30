#include <commons/Game/Game.hpp>
#include <commons/Game/GameConfig.hpp>
#include <apis/Title/TitleApi.hpp>

#include <core/Data/MusicData/MusicData.hpp>
#include <core/Data/CourseData/CourseData.hpp>
#include <core/Data/TapSE/TapSE.hpp>
#include <utils/Audio/Loop.hpp>
#include <scenes/Scene/Title/TitleScene.hpp>
#include <scenes/Scene/FileLoad/FileLoadScene.hpp>
#include <scenes/Scene/Tutorial/TutorialScene.hpp>
#include <scenes/Scene/Config/ConfigScene.hpp>
#include <scenes/Scene/MusicSelect/MusicSelectScene.hpp>
#include <scenes/Scene/Main/MainScene.hpp>
#include <scenes/Scene/Result/ResultScene.hpp>
#include <scenes/Scene/CourseSelect/CourseSelectScene.hpp>
#include <scenes/Scene/Course/CourseScene.hpp>
#include <scenes/Scene/Download/DownloadScene.hpp>

#include <utils/Addon/IntervalCounter.hpp>
#include <Siv3D.hpp>

namespace
{
	/// <summary>
	/// Textureの登録
	/// </summary>
	void LoadTexture()
	{
		// ロード
		TextureAsset::Register(U"canvasBg", U"Resource/Img/Load/bg.png");

		//タイトル
		TextureAsset::Register(U"titleBgs", U"Resource/Img/Title/title_bgs.png");
		TextureAsset::Register(U"logo", U"Resource/Img/Title/logo.png");
		TextureAsset::Register(U"modeBack", U"Resource/Img/Title/modeBack.png");
		TextureAsset::Register(U"titleIcons", U"Resource/Img/Title/title_icons.png");

		//キーコン
		TextureAsset::Register(U"keyconBG2", U"Resource/Img/Config/keyconBG2.png");

		//選曲
		TextureAsset::Register(U"sticky", U"Resource/Img/MusicSelect/sticky.png");
		TextureAsset::Register(U"sticky_red", U"Resource/Img/MusicSelect/sticky_red.png");
		TextureAsset::Register(U"line", U"Resource/Img/MusicSelect/line.png");
		TextureAsset::Register(U"memo", U"Resource/Img/MusicSelect/memo.png");
		TextureAsset::Register(U"memoCourse", U"Resource/Img/MusicSelect/memo_course.png");
		TextureAsset::Register(U"memoDL", U"Resource/Img/MusicSelect/memo_dl.png");
		TextureAsset::Register(U"favorite", U"Resource/Img/MusicSelect/favorite.png");

		TextureAsset::Register(U"levelbg", U"Resource/Img/MusicSelect/select.png");
		TextureAsset::Register(U"levelMask", U"Resource/Img/MusicSelect/levelmask.png");

		//ジャンルアイコン
		TextureAsset::Register(U"genre_level1", U"Resource/Img/MusicSelect/icon/level1.png");
		TextureAsset::Register(U"genre_level2", U"Resource/Img/MusicSelect/icon/level2.png");
		TextureAsset::Register(U"genre_level3", U"Resource/Img/MusicSelect/icon/level3.png");
		TextureAsset::Register(U"genre_level4", U"Resource/Img/MusicSelect/icon/level4.png");
		TextureAsset::Register(U"genre_level5", U"Resource/Img/MusicSelect/icon/level5.png");
		TextureAsset::Register(U"genre_level_star", U"Resource/Img/MusicSelect/icon/level_star.png");
		TextureAsset::Register(U"genre_file", U"Resource/Img/MusicSelect/icon/file.png");
		TextureAsset::Register(U"genre_all", U"Resource/Img/MusicSelect/icon/red.png");
		TextureAsset::Register(U"genre_favorite", U"Resource/Img/MusicSelect/icon/favorite.png");

		// dl_file
		TextureAsset::Register(U"dl_file", U"Resource/Img/MusicSelect/icon/dl_file.png");

		//リザルトシーン
		TextureAsset::Register(U"AAA", U"Resource/Img/Result/rank/AAA.png");
		TextureAsset::Register(U"AA", U"Resource/Img/Result/rank/AA.png");
		TextureAsset::Register(U"A", U"Resource/Img/Result/rank/A.png");
		TextureAsset::Register(U"B", U"Resource/Img/Result/rank/B.png");
		TextureAsset::Register(U"C", U"Resource/Img/Result/rank/C.png");
		TextureAsset::Register(U"D", U"Resource/Img/Result/rank/D.png");
		TextureAsset::Register(U"E", U"Resource/Img/Result/rank/E.png");
		TextureAsset::Register(U"F", U"Resource/Img/Result/rank/F.png");
		TextureAsset::Register(U"iconAP", U"Resource/Img/Result/rank/allperfects.png");
		TextureAsset::Register(U"iconFC", U"Resource/Img/Result/rank/fullcombos.png");
		TextureAsset::Register(U"iconClear", U"Resource/Img/Result/rank/clears.png");

		TextureAsset::Register(U"newRecord", U"Resource/Img/Result/newrecord.png");

		//メイン
		//portrait
		TextureAsset::Register(U"tapEffect2", U"Resource/Img/Main/tapeffect2.png");

		TextureAsset::Register(U"portrait_notes9", U"Resource/Img/Main/ノーツ/portrait/portrait_notes.png");
		TextureAsset::Register(U"portrait_notes1", U"Resource/Img/Main/ノーツ/portrait/portrait_notes1.png");
		TextureAsset::Register(U"portrait_notes2", U"Resource/Img/Main/ノーツ/portrait/portrait_notes2.png");
		TextureAsset::Register(U"portrait_notes3", U"Resource/Img/Main/ノーツ/portrait/portrait_notes3.png");
		TextureAsset::Register(U"portrait_notes4", U"Resource/Img/Main/ノーツ/portrait/portrait_notes4.png");
		TextureAsset::Register(U"portrait_notes5", U"Resource/Img/Main/ノーツ/portrait/portrait_notes5.png");
		TextureAsset::Register(U"portrait_notes6", U"Resource/Img/Main/ノーツ/portrait/portrait_notes6.png");
		TextureAsset::Register(U"portrait_notes7", U"Resource/Img/Main/ノーツ/portrait/portrait_notes7.png");
		TextureAsset::Register(U"portrait_notes10", U"Resource/Img/Main/ノーツ/portrait/portrait_notes10.png");

		TextureAsset::Register(U"mainbg_front", U"Resource/Img/Main/back1.png");
		TextureAsset::Register(U"mainbg_back", U"Resource/Img/Main/back2.png");

		//隕石
		TextureAsset::Register(U"note_black", U"Resource/Img/Main/ノーツ/通常/kuro.png");
		TextureAsset::Register(U"note_red", U"Resource/Img/Main/ノーツ/通常/aka.png");
		TextureAsset::Register(U"note_blue", U"Resource/Img/Main/ノーツ/通常/ao.png");
		TextureAsset::Register(U"note_yellow", U"Resource/Img/Main/ノーツ/通常/ki.png");
		TextureAsset::Register(U"note_orange", U"Resource/Img/Main/ノーツ/通常/daidai.png");
		TextureAsset::Register(U"note_green", U"Resource/Img/Main/ノーツ/通常/midori.png");
		TextureAsset::Register(U"note_purple", U"Resource/Img/Main/ノーツ/通常/murasaki.png");
		TextureAsset::Register(U"note_white", U"Resource/Img/Main/ノーツ/通常/siro.png");
		TextureAsset::Register(U"note_rainbow", U"Resource/Img/Main/ノーツ/通常/rainbow.png");
		//彗星
		TextureAsset::Register(U"comet_red", U"Resource/Img/Main/ノーツ/ロング/aka.png");
		TextureAsset::Register(U"comet_blue", U"Resource/Img/Main/ノーツ/ロング/ao.png");
		TextureAsset::Register(U"comet_yellow", U"Resource/Img/Main/ノーツ/ロング/ki.png");
		TextureAsset::Register(U"comet_orange", U"Resource/Img/Main/ノーツ/ロング/daidai.png");
		TextureAsset::Register(U"comet_green", U"Resource/Img/Main/ノーツ/ロング/midori.png");
		TextureAsset::Register(U"comet_purple", U"Resource/Img/Main/ノーツ/ロング/murasaki.png");
		TextureAsset::Register(U"comet_black", U"Resource/Img/Main/ノーツ/ロング/kuro.png");
		TextureAsset::Register(U"comet_rainbow_head", U"Resource/Img/Main/ノーツ/ロング/rainbow_head.png");
		TextureAsset::Register(U"comet_rainbow_tail", U"Resource/Img/Main/ノーツ/ロング/rainbow_tail.png");

		//
		TextureAsset::Register(U"center_base", U"Resource/Img/Main/center/base.png");
		TextureAsset::Register(U"center_redlight", U"Resource/Img/Main/center/redlight.png");
		TextureAsset::Register(U"center_bluelight", U"Resource/Img/Main/center/bluelight.png");
		TextureAsset::Register(U"center_yellowlight", U"Resource/Img/Main/center/yellowlight.png");
		TextureAsset::Register(U"center_1", U"Resource/Img/Main/center/red.png");
		TextureAsset::Register(U"center_2", U"Resource/Img/Main/center/blue.png");
		TextureAsset::Register(U"center_3", U"Resource/Img/Main/center/yellow.png");
		TextureAsset::Register(U"center_4", U"Resource/Img/Main/center/green.png");
		TextureAsset::Register(U"center_5", U"Resource/Img/Main/center/orange.png");
		TextureAsset::Register(U"center_6", U"Resource/Img/Main/center/purple.png");
		TextureAsset::Register(U"center_7", U"Resource/Img/Main/center/white.png");

		//バー
		TextureAsset::Register(U"barBase", U"Resource/Img/Main/bar/base.png");
		TextureAsset::Register(U"bar1", U"Resource/Img/Main/bar/bar1.png");
		TextureAsset::Register(U"bar2", U"Resource/Img/Main/bar/bar2.png");
		TextureAsset::Register(U"streamPosBase", U"Resource/Img/Main/bar/streamPosBase.png");

		TextureAsset::Register(U"tapEffct", U"Resource/Img/Main/prasma.png");

		TextureAsset::Register(U"combo", U"Resource/Img/Main/combo.png");

		// main start
		TextureAsset::Register(U"start01", U"Resource/Img/Main/start/01.png");
		TextureAsset::Register(U"start02", U"Resource/Img/Main/start/02.png");
		TextureAsset::Register(U"start03", U"Resource/Img/Main/start/03.png");
		TextureAsset::Register(U"start04", U"Resource/Img/Main/start/04.png");
		TextureAsset::Register(U"start05", U"Resource/Img/Main/start/05.png");
		TextureAsset::Register(U"start06", U"Resource/Img/Main/start/06.png");
		TextureAsset::Register(U"start07", U"Resource/Img/Main/start/07.png");

		// resut
		TextureAsset::Register(U"memo2", U"Resource/Img/Result/memo2.png");

		TextureAsset::Register(U"fcAnime", U"Resource/Img/Result/fcAnime.png");
		TextureAsset::Register(U"apAnime", U"Resource/Img/Result/apAnime.png");
		TextureAsset::Register(U"fcLogo", U"Resource/Img/Result/fc.png");
		TextureAsset::Register(U"apLogo", U"Resource/Img/Result/ap.png");
		TextureAsset::Register(U"pass", U"Resource/Img/Result/pass.png");
		TextureAsset::Register(U"noPass", U"Resource/Img/Result/noPass.png");
	}
	void LoadSound()
	{
		// BGM
		AudioAsset::Register({ U"title", { U"System"} }, U"Resource/Sound/BGM/title.mp3", ct::MakeLoopTiming(10.66s, 160.0s));
		AudioAsset::Register({ U"result" , {U"System"} }, U"Resource/Sound/BGM/result.mp3", ct::MakeLoopTiming(6s, 90.0s));

		// SE
		AudioAsset::Register({ U"select",{ U"System" } }, U"Resource/Sound/SE/select.ogg");
		AudioAsset::Register({U"desisionSmall",{ U"System" } }, U"Resource/Sound/SE/decide.mp3");
		AudioAsset::Register({U"desisionLarge",{ U"System" } }, U"Resource/Sound/SE/mekuru.mp3");
		AudioAsset::Register({U"desisionLarge2",{ U"System" } }, U"Resource/Sound/SE/ironuri.mp3");
		AudioAsset::Register({U"cancel",{ U"System" } }, U"Resource/Sound/SE/cancel.mp3");
	}
	void LoadFont()
	{
		const FilePathView straightPath = U"Resource/Font/Straightfont.ttf";

		// NOTE: 旧Siv3D移植の都合でネーミングと数値に差異がある

		FontAsset::Register(U"s-r-8", 8 + 8, straightPath);
		FontAsset::Register(U"s-r-10", 10 + 10, straightPath);
		FontAsset::Register(U"s-o-10", FontMethod::SDF, 10 + 10, straightPath, FontStyle::Bold);
		FontAsset(U"s-o-10").setBufferThickness(3);
		FontAsset::Register(U"s-o-12", FontMethod::SDF, 12 + 11, straightPath, FontStyle::Bold);
		FontAsset(U"s-o-12").setBufferThickness(3);
		FontAsset::Register(U"s-b-15", 15, straightPath, FontStyle::Bold);
		FontAsset::Register(U"s-b-20", 20 + 20, straightPath, FontStyle::Bold);

		FontAsset::Register(U"r-12", 12 + 5);
		FontAsset::Register(U"r-13", 13 + 5);
		FontAsset::Register(U"r-20", 20 + 10);
		FontAsset::Register(U"r-30", 30 + 15);
		FontAsset::Register(U"b-10", 10 + 5, Typeface::Regular, FontStyle::Bold);
		FontAsset::Register(U"b-16", 16 + 7, Typeface::Bold);
		FontAsset::Register(U"i-15", 15 + 5, Typeface::Regular, FontStyle::Italic);

		FontAsset::Register(U"fallback", FontMethod::MSDF, 18);
		FontAsset fallback(U"fallback");
		Print.getFont().addFallback(fallback);
	}

	void AssetLoad()
	{
		::LoadFont();
		::LoadTexture();
		::LoadSound();
	}
}
namespace ct
{
    class Game::Impl
    {
	public:
		Impl()
		{
			this->init();
		}
		void startUp()
		{
			m_scene.init(SceneName::Load, 1000);
		}
		bool updateAndDraw()
		{
			if (!m_scene.update()) {
				return false;
			}
			return true;
		}
    public:
        Array<MusicData> m_musics;
        Array<CourseData> m_courses;
        Array<TapSE> m_tapSEs;

        GameConfig m_config;
	private:
		void registerScene()
		{
			m_scene.add<FileLoadScene>(SceneName::Load);
			m_scene.add<TitleScene>(SceneName::Title);
			m_scene.add<ConfigScene>(SceneName::Config);
			m_scene.add<TutorialScene>(SceneName::Tutorial);
			m_scene.add<MusicSelectScene>(SceneName::Select);
			m_scene.add<MainScene>(SceneName::Main);
			m_scene.add<ResultScene>(SceneName::Result);
			m_scene.add<CourseSelectScene>(SceneName::CourseSelect);
			m_scene.add<CourseScene>(SceneName::Course);
			m_scene.add<DownloadScene>(SceneName::Download);
		}
		void registerAddon()
		{
			IntervalCounter::RegisterAddon();
		}
		void init()
		{
			this->registerAddon();
			this->registerScene();
			::AssetLoad();
		}
	private:
		MyApp m_scene; //シーン
	};
    Game::Game() :
        m_pImpl(std::make_unique<Impl>())
    {
    }
    Game::~Game()
    {
    }

    void Game::Startup()
    {
        TitleApi::CheckVersion();

        //ゲームコンフィグの初期化
        //キー入力やハイスピ等
        Config().init();

		Instance()->m_pImpl->startUp();
    }

    bool Game::Update()
    {
		return Instance()->m_pImpl->updateAndDraw();
    }
    void Game::Shutdown()
    {
        Config().save();
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
}
