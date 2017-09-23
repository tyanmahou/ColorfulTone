#pragma once
#include"Singleton.h"
# include <Siv3D.hpp>
# include <HamFramework.hpp>
#include"MusicData.h"
#include"CourseData.h"

#include"Score.h"
#include"GameConfig.h"

#include"SoundManager.h"
//���L�f�[�^
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


//�Q�[���N���X
class Game :public Singleton<Game>
{
public:
	static constexpr auto Version = L"Ver2.1.0";
private:

	MyApp m_scene;			//�V�[��


	void init();			//������

	Game():m_isMusicLoadEnd(false)
	{ 
		init();
	};		//�R���X�g���N�^


public:
	Array<MusicData> m_musics;
	Array<CourseData> m_courses;
	Array<String> m_tapSEPaths;

	Effect m_effect;		//�G�t�F�N�g
	GameConfig m_config;

	bool m_isMusicLoadEnd;


	bool updateAndDraw();	//�X�V�ƕ`��

	friend Singleton < Game >;
};

void AssetLoad();


