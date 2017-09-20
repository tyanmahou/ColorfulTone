#pragma once
#include"PlayMusicGame.h"
#include"HighSpeedDemo.h"

class Preview;

class SoundPlayer
{
private:

	GUI m_gui;

	bool m_isActive;

	const Sound* m_sound=nullptr;

	Preview*const m_scene;


public:
	SoundPlayer(Preview* scene) :
		m_gui(GUIStyle::Default),
		m_isActive(true),
		m_scene(scene)
	{
		m_gui.style.background.color = Color(255, 255, 255, 100);
		m_gui.add(L"PlayButton", GUIButton::Create(L"プレイ",false));
		m_gui.add(L"StopButton", GUIButton::Create(L"停止",false));
		m_gui.add(L"UpdateButton", GUIButton::Create(L"更新", false));

		m_gui.addln(L"OpenButton", GUIButton::Create(L"開く"));

		m_gui.add(L"LevelText", GUIText::Create(L"譜面:"));
		m_gui.add(L"LevelName", GUIText::Create(L"none"));
		m_gui.addln(L"LevelSlider", GUISlider::Create(0.0, 1.0, 0.0, 400));

		m_gui.add(L"CountText", GUIText::Create(L"位置"));
		m_gui.add(L"Slider", GUISlider::Create(0.0, 1.0, 0.0, 500));

		m_gui.setPos((Window::Width() - m_gui.getRect().w) / 2, 360);
	}

	void update();


	void setSound(const Sound& sound)
	{
		m_sound = &sound;
	}

	void setLevelName(const String& name)
	{
		m_gui.text(L"LevelName").text = name;
	}

};
class Preview
{
private:

	Optional<FilePath> m_dirPath;

	HighSpeedDemo m_highSpeedDemo;

	SoundPlayer m_soundPlayer;

	PlayMusicGame m_musicGame;

	Optional<MusicData> m_musicData;

	int m_selectLevel = 0;
	bool m_isPlay=false;
	double m_count = 0;
	float m_scrollRate=1.0f;

	void loadProject(Optional<FilePath>& path);

	void playOrStop()
	{
		if (m_musicData.has_value())
		{
			m_isPlay ^=true;
			m_musicGame.notesInit();
		}

	}

public:
	Preview() :
		m_soundPlayer(this)
	{}

	void init()
	{
		SoundAsset(L"title").stop();
	}

	void notify(const String& event);

	void update();
	void draw()const
	{
		if (m_musicData.has_value())
		{
			if (m_isPlay)
			{
				m_musicGame.draw(*m_musicData);
			}
			else
			{
				m_musicGame.previewDraw(*m_musicData,m_count);
			}
			m_highSpeedDemo.draw(m_musicData->getMinSoundBeat(), m_musicData->getMaxSoundBeat(), m_scrollRate);
			m_musicGame.drawCurrentBPM();
		}

	}

	bool isPlaying()const
	{
		return m_isPlay;
	}

	void setLevelRate(double rate)
	{
		if (m_musicData.has_value())
		{
			auto& notes = m_musicData->getNotesData();
			m_selectLevel = std::min(notes.size() - 1, (unsigned)(m_musicData->getNotesData().size()*rate));
			
			m_soundPlayer.setLevelName(notes[m_selectLevel].getLevelName());

		}

	}


};
