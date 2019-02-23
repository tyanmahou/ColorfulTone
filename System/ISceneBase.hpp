﻿#pragma once

# include <Siv3D.hpp>
# include <HamFramework.hpp>
# include"NotesData.h"

//共有データ
struct GameData
{
	NotesData m_nowNotes;

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
		m_data->m_fromScene = std::move(m_data->m_toScene);
		m_data->m_toScene = state;
		this->finally();
		this->MyApp::Scene::changeScene(state, transitionTimeMillisec, crossFade);
	}
};

namespace SceneName
{
	constexpr auto Load = L"load";
	constexpr auto Title = L"title";
	constexpr auto Select = L"select";
	constexpr auto Main = L"main";
	constexpr auto Result = L"result";
	constexpr auto CourseSelect = L"courseSelect";
	constexpr auto Course = L"course";
	constexpr auto Config = L"config";
	constexpr auto Tutorial = L"tutorial";
	constexpr auto Download = L"download";
}