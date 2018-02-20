#pragma once
#include "Game.h"
#include "HighSpeedDemo.h"
#include<future>
enum class SortMode
{
	FileName,
	MusicName,
	ArtistName,
	Default = FileName
};

class MusicSelectScene :public MyApp::Scene
{
private:

	enum class Mode :unsigned int
	{
		GenreSelect,
		MusicSelect,
		LevelSelect,
	}m_mode;
	int m_timer;				//タイマー

	Font m_font1;
	Font m_font2;
	Font m_font3;
	Font m_rateFont;

	static unsigned int m_selectMusic;			//選択中の楽曲
	static unsigned int m_selectLevel;			//選択中の譜面難易度
	static unsigned int m_selectGenre;			//選択中のジャンル
	static bool m_levelInfoMode;				//表示情報
	static SortMode m_sortMode;					//ソートモード


	mutable Array<MusicData> m_musics;
	HighSpeedDemo m_highSpeedDemo;
	unsigned int m_musicsSize;		//楽曲数


	bool m_changeMainScene;

	void sort();

	void musicBannerAndInfoDraw()const;
	void genreBannerAndInfoDraw()const;
	void levelSelectDraw()const;
	void highSpeedDraw()const;

	//演出用Easing
	void EasingStartBySelectMusic();
	void EasingEndBySelectMusic();
	EasingController<int> m_rightShift;
	EasingController<int> m_leftShift;
	EasingController<int> m_bannerSize;
	EasingController<Vec2> m_bannerBGSize;
	EasingController<int> m_bannerBGY;
	EasingController<int> m_infoSeat;
	EasingController<int> m_isMusicDefineEasing;
	EasingController<int> m_isGenreDefineEasing;


public:

	static std::future<void> m_audioResult;


	MusicSelectScene();
	~MusicSelectScene();

	void init() override;
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
};
