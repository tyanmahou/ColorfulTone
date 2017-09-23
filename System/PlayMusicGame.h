#pragma once

#include"FCAPAnimeManager.h"
#include"Score.h"
#include"AudioSpectrum.h"

class PlayMusicGame
{

private:

	bool m_isCourse = false;
	float m_initRate = 100.0f;
	float m_life = 100.0f;

	Sound m_sound;				//�Đ������
	String m_soundNameID;
	NotesData m_notesData;		//���ʏ��

	FCAPANimeManager m_FCAPAnime;	//�t���R��AP�A�j��
	Score m_score;

	float m_scrollRate;			//�X�N���[�����[�g
	int m_totalNotes;			//�g�[�^���m�[�c��
	uint64 m_finishSample;		//�I���̃T���v�����O�ʒu


	String m_title;				//�A�[�e�B�X�g���{�Ȗ�

								//���t�J�n�ƏI��
	bool m_isStart;
	bool m_isFinish;
	EasingController<int> m_barXEasing;

	double m_nowCount;			//���݂̃J�E���g

	std::shared_ptr<IPlayBG> m_playBG;
	AudioSpectrum m_spectrum;
	void uiDraw()const;
public:

	static Effect& GetEffect()
	{
		static Effect effect;

		return effect;
	}


	PlayMusicGame();

	void init(MusicData& nowMusic, const int level, const float scrollRate);

	void synchroCount(double& count)
	{
		m_notesData.synchroCount(m_sound, count);
	}
	void update();

	void drawBG(const MusicData & nowMusic, const double drawCount)const;

	void draw(const MusicData & nowMusic)const;

	void previewDraw(const MusicData & nowMusic,const double count)const;

	void drawCurrentBPM()const
	{
		String tmp = Format(m_notesData.getCurrentBPM(), L"*", m_scrollRate);

		if (const auto result = EvaluateOpt(tmp))
		{
			PutText(tmp+L"=",result.value()).from(20, Window::Height()-100);
		}

	}

	inline bool isFinish()const
	{
		return m_isFinish && (m_FCAPAnime.isEnd() || !m_FCAPAnime.isStart());
	}

	inline const Score& getScore()const
	{
		return m_score;
	}
	inline const Sound& getSound()const
	{
		return m_sound;
	}

	inline void stopSound(const SecondsF& fadeoutDuration = SecondsF(2.0))
	{
		m_sound.stop(fadeoutDuration);
	}
	inline void setScrollRate(float rate)
	{
		m_scrollRate = rate;
	}
	void notesInit()
	{
		//���ʂ̏�����
		m_notesData.init();
		m_score = Score();
		m_isFinish = false;
	}

	inline const NotesData& getNotesData()const
	{
		return m_notesData;
	}

	void setCourseMode(float life)
	{
		m_isCourse = true;
		m_initRate = life;
		m_life = life;
	}
	float getLife()
	{
		return m_life;
	}
	float getInitRate()
	{
		return m_initRate;
	}

};
