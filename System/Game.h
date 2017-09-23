#pragma once
#include"Singleton.h"
# include <Siv3D.hpp>
# include <HamFramework.hpp>
#include"MusicData.h"
#include"CourseData.h"

#include"Score.h"
#include"GameConfig.h"

#include"SoundManager.h"
//共有データ
struct GameData
{
	MusicData m_nowMusics;
	int m_selectMusic;
	int m_selectLevel;
	float m_scrollRate;

	Score m_resultScore;


	bool m_isCoursePlay = false;
	int m_currentCourseIndex = 0;
	int m_selectCourse=0;
	float m_life = 100;
};


using MyApp = SceneManager<String, GameData>;


//ゲームクラス
class Game :public Singleton<Game>
{
public:
	static constexpr auto Version = L"Ver2.1.0";
private:

	MyApp m_scene;			//シーン


	void init();			//初期化

	Game():m_isMusicLoadEnd(false)
	{ 
		init();
	};		//コンストラクタ


public:
	Array<MusicData> m_musics;
	Array<CourseData> m_courses;
	Array<String> m_tapSEPaths;

	Effect m_effect;		//エフェクト
	GameConfig m_config;

	bool m_isMusicLoadEnd;


	bool updateAndDraw();	//更新と描画

	friend Singleton < Game >;
};

void AssetLoad();


