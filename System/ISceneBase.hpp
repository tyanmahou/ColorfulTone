#pragma once

# include <Siv3D.hpp>
# include <HamFramework.hpp>
# include"MusicData.h"

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
	int m_selectCourse = 0;
	float m_life = 100;
	String m_fromScene;
	String m_toScene = L"load";
};


using MyApp = SceneManager<String, GameData>;

class ISceneBase : public MyApp::Scene
{
protected:
	virtual void finally() {};
	void changeScene(const String& state, int transitionTimeMillisec = 1000, bool crossFade = true)
	{
		this->finally();
		m_data->m_fromScene = std::move(m_data->m_toScene);
		m_data->m_toScene = state;
		this->MyApp::Scene::changeScene(state, transitionTimeMillisec, crossFade);
	}
};