#pragma once
#include"Game.h"
#include"MusicSelectView.hpp"
#include "HighSpeedDemo.h"
#include"Audition.hpp"
class MusicSelect :public MyApp::Scene
{
public:
	enum class SortMode : uint8
	{
		FileName,
		MusicName,
		ArtistName,
		LastUpdateAt,
		Default = FileName
	};
	struct SelectMusicsInfo
	{
		uint32 genre = 0;	// 選択中のジャンル
		uint32 music = 0;	// 選択中の曲
		uint32 level = 0;	// 選択中のレベル
		SortMode sortMode = SortMode::Default; // 選択中のソート
	};
	enum class Action :uint8
	{
		GenreSelect,
		MusicSelect,
		LevelSelect,
	};
private:
	Action m_action = Action::MusicSelect;
	int m_moveSelect = 0;

	Array<MusicData> m_musics;
	MusicSelectView m_view;
	Audition m_audition;
	HighSpeedDemo m_highSpeedDemo;
	double m_shaderTimer = 0.0;
public:
	MusicSelect();
	~MusicSelect();

	void init() override;
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;

	//
	static SelectMusicsInfo GetSelectInfo();
	const Array<MusicData>& getMusics()const
	{
		return m_musics;
	}
	double getShaderTimer()const
	{
		return m_shaderTimer;
	}
	Action getAction()const
	{
		return m_action;
	}
	int getMoveSelect()const
	{
		return m_moveSelect;
	}
	const HighSpeedDemo& getHighSpeedDemo()const
	{
		return m_highSpeedDemo;
	}
	float getScrollRate()const
	{
		return m_data->m_scrollRate;
	}
};

