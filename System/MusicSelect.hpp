#pragma once
#include"Game.h"
#include"MusicSelectView.hpp"
#include "HighSpeedDemo.h"
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
private:
	Array<MusicData> m_musics;
	MusicSelectView m_view;
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
};

