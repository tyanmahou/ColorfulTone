#include"Game.h"
#include"TitleScene.h"
#include"FileLoad.h"
#include"Tutorial.h"
#include"KeyConfigScene.h"
#include"MusicSelectScene.h"
#include"MainScene.h"
#include"ResultScene.h"
#include"VideoAsset.h"
#include"CourseScene.h"
#include"CoursePlay.h"
//--------------------------------------------------------------------------------
//�֐��FLoadTexture
//--------------------------------------------------------------------------------
//�T�v�F�摜�̓o�^
//--------------------------------------------------------------------------------

void loadTexture()
{

	TextureAsset::Register(L"label", L"Resource/Img/label.png");

	//�^�C�g��
	TextureAsset::Register(L"logo", L"Resource/Img/Title/logo.png");
	TextureAsset::Register(L"modeBack", L"Resource/Img/Title/modeBack.png");

	TextureAsset::Register(L"iconPlay",     L"Resource/Img/Title/TitleIcon/play.png");
	TextureAsset::Register(L"iconConfig", L"Resource/Img/Title/TitleIcon/config.png");
	TextureAsset::Register(L"iconReset", L"Resource/Img/Title/TitleIcon/reset.png");
	TextureAsset::Register(L"iconShutdown", L"Resource/Img/Title/TitleIcon/shutdown.png");
	TextureAsset::Register(L"iconTutorial", L"Resource/Img/Title/TitleIcon/tutorial.png");
	TextureAsset::Register(L"iconPlayCourse", L"Resource/Img/Title/TitleIcon/playCourse.png");
	TextureAsset::Register(L"iconAccess", L"Resource/Img/Title/TitleIcon/access.png");


	//�L�[�R��
	TextureAsset::Register(L"keyconBG", L"Resource/Img/KeyConfig/keyconBG.png");
	TextureAsset::Register(L"keyconBG2", L"Resource/Img/KeyConfig/keyconBG2.png");



	//�I��
	TextureAsset::Register(L"bannerbg", L"Resource/Img/MusicSelect/panel.png");
	TextureAsset::Register(L"selectbg", L"Resource/Img/MusicSelect/selectBG.png");
	TextureAsset::Register(L"levelbg", L"Resource/Img/MusicSelect/select.png");
	TextureAsset::Register(L"levelMask", L"Resource/Img/MusicSelect/levelmask.png");

	TextureAsset::Register(L"coursebg", L"Resource/Img/MusicSelect/courseBG.png");


	//�W�������A�C�R��
	TextureAsset::Register(L"genre_level1", L"Resource/Img/MusicSelect/icon/level1.png");
	TextureAsset::Register(L"genre_level2", L"Resource/Img/MusicSelect/icon/level2.png");
	TextureAsset::Register(L"genre_level3", L"Resource/Img/MusicSelect/icon/level3.png");
	TextureAsset::Register(L"genre_level4", L"Resource/Img/MusicSelect/icon/level4.png");
	TextureAsset::Register(L"genre_level5", L"Resource/Img/MusicSelect/icon/level5.png");
	TextureAsset::Register(L"genre_file",   L"Resource/Img/MusicSelect/icon/file.png");
	TextureAsset::Register(L"genre_all",    L"Resource/Img/MusicSelect/icon/red.png");






	//���U���g�V�[��
	TextureAsset::Register(L"resultBG1", L"Resource/Img/Result/resultBG1.png");
	TextureAsset::Register(L"resultBG2", L"Resource/Img/Result/resultBG2.png");
	TextureAsset::Register(L"resultClear", L"Resource/Img/Result/clear.png");
	TextureAsset::Register(L"resultFailed", L"Resource/Img/Result/failed.png");

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

	//���C��
	TextureAsset::Register(L"mainbg", L"Resource/Img/Main/back.png");
	//覐�
	TextureAsset::Register(L"note_black", L"Resource/Img/Main/�m�[�c/�ʏ�/kuro.png");
	TextureAsset::Register(L"note_red",   L"Resource/Img/Main/�m�[�c/�ʏ�/aka.png");
	TextureAsset::Register(L"note_blue",  L"Resource/Img/Main/�m�[�c/�ʏ�/ao.png");
	TextureAsset::Register(L"note_yellow",L"Resource/Img/Main/�m�[�c/�ʏ�/ki.png");
	TextureAsset::Register(L"note_orange",L"Resource/Img/Main/�m�[�c/�ʏ�/daidai.png");
	TextureAsset::Register(L"note_green", L"Resource/Img/Main/�m�[�c/�ʏ�/midori.png");
	TextureAsset::Register(L"note_purple",L"Resource/Img/Main/�m�[�c/�ʏ�/murasaki.png");
	TextureAsset::Register(L"note_white", L"Resource/Img/Main/�m�[�c/�ʏ�/siro.png");
	TextureAsset::Register(L"note_rainbow", L"Resource/Img/Main/�m�[�c/�ʏ�/rainbow.png");
	//�a��
	TextureAsset::Register(L"comet_red",    L"Resource/Img/Main/�m�[�c/�����O/aka.png");
	TextureAsset::Register(L"comet_blue",   L"Resource/Img/Main/�m�[�c/�����O/ao.png");
	TextureAsset::Register(L"comet_yellow", L"Resource/Img/Main/�m�[�c/�����O/ki.png");
	TextureAsset::Register(L"comet_orange", L"Resource/Img/Main/�m�[�c/�����O/daidai.png");
	TextureAsset::Register(L"comet_green",  L"Resource/Img/Main/�m�[�c/�����O/midori.png");
	TextureAsset::Register(L"comet_purple", L"Resource/Img/Main/�m�[�c/�����O/murasaki.png");
	TextureAsset::Register(L"comet_black", L"Resource/Img/Main/�m�[�c/�����O/kuro.png");
	TextureAsset::Register(L"comet_rainbow_head", L"Resource/Img/Main/�m�[�c/�����O/rainbow_head.png");
	TextureAsset::Register(L"comet_rainbow_tail", L"Resource/Img/Main/�m�[�c/�����O/rainbow_tail.png");

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


	//�o�[
	TextureAsset::Register(L"barBase", L"Resource/Img/Main/bar/base.png");
	TextureAsset::Register(L"bar1", L"Resource/Img/Main/bar/bar1.png");
	TextureAsset::Register(L"bar2", L"Resource/Img/Main/bar/bar2.png");
	TextureAsset::Register(L"streamPosBase", L"Resource/Img/Main/bar/streamPosBase.png");
	
	TextureAsset::Register(L"tapEffct", L"Resource/Img/Main/prasma.png");

	TextureAsset::Register(L"combo", L"Resource/Img/Main/combo.png");
	TextureAsset::PreloadAll();

	TextureAsset::Register(L"fcAnime", L"Resource/Img/Result/fcAnime.png");
	TextureAsset::Register(L"apAnime", L"Resource/Img/Result/apAnime.png");
	TextureAsset::Register(L"fcLogo", L"Resource/Img/Result/fc.png");
	TextureAsset::Register(L"apLogo", L"Resource/Img/Result/ap.png");
	TextureAsset::Register(L"backLight", L"Resource/Img/Result/backLight.png");
	TextureAsset::Register(L"pass", L"Resource/Img/Result/pass.png");
	TextureAsset::Register(L"noPass", L"Resource/Img/Result/noPass.png");

	Mahou::VideoAsset::Register(L"titleBG",L"Resource/Movie/titleBG.avi", true);
	Mahou::VideoAsset::Register(L"loading",L"Resource/Movie/loading.avi", true);
	Mahou::VideoAsset::Register(L"start", L"Resource/Movie/start.avi", false);

}
//--------------------------------------------------------------------------------
//�֐��FLoadSound
//--------------------------------------------------------------------------------
//�T�v�F���̓o�^
//--------------------------------------------------------------------------------

void loadSound()
{

#pragma region BGM 

	SoundAsset::Register(L"title",L"Resource/Sound/BGM/title.wav",SoundLoop(true), { L"System" });

	SoundManager::BGM::Register(L"title");
	SoundAsset::Register(L"result", L"Resource/Sound/BGM/result.wav", SoundLoop(true),{ L"System" });
	SoundManager::BGM::Register(L"result");



#pragma endregion BGM�̃��[�h

#pragma region SE

	SoundAsset::Register(L"select", L"Resource/Sound/SE/Select.wav", { L"System" });
	SoundAsset::Register(L"desisionSmall", L"Resource/Sound/SE/Desision_Small.wav", { L"System" });
	SoundAsset::Register(L"desisionLarge", L"Resource/Sound/SE/Desision_Large.wav", { L"System" });

	SoundAsset::Register(L"cancel", L"Resource/Sound/SE/cancel.wav", { L"System" });
#pragma endregion SE�̃��[�h

	SoundAsset::PreloadByTag(L"System");

}


//--------------------------------------------------------------------------------
//�֐��FInit
//--------------------------------------------------------------------------------
//�T�v�F�Q�[���̏�����
//--------------------------------------------------------------------------------

void Game::init(){

	m_scene.add<FileLoad>(L"load");
	m_scene.add<TitleScene>(L"title");
	m_scene.add<ConfigScene>(L"config");
	m_scene.add<Tutorial>(L"tutorial");
	m_scene.add<MusicSelectScene>(L"select");
	m_scene.add<MainScene>(L"main");
	m_scene.add<ResultScene>(L"result");
	m_scene.add<CourseSelectScene>(L"courseSelect");
	m_scene.add<CoursePlay>(L"course");

	AssetLoad();
}

bool Game::updateAndDraw(){

	if (!m_scene.updateAndDraw())
		return false;
	return true;
}

void AssetLoad()
{

	FontManager::Register(L"Resource/Font/Straightfont.ttf");

	FontAsset::Register(L"combo", 10, L"Straight");

	FontAsset::Register(L"label", 15, L"Straight", FontStyle::Italic);
	FontAsset::Register(L"judge", 8, L"Straight");
	FontAsset::Register(L"info", 10, Typeface::Default, FontStyle::Bold);
	FontAsset::Register(L"TextObject", 17, Typeface::Default, FontStyle::Outline);
	FontAsset(L"TextObject").changeOutlineStyle(TextOutlineStyle(Palette::Black, Palette::White, 1));

	FontAsset::Register(L"configTitle", 15,Typeface::Default,FontStyle::Italic);

	FontAsset::Register(L"configSelect", 13);


	loadTexture();

	loadSound();


}
