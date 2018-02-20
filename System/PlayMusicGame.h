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

	Sound m_sound;				//再生する曲
	String m_soundNameID;
	NotesData m_notesData;		//譜面情報

	FCAPANimeManager m_FCAPAnime;	//フルコンAPアニメ
	Score m_score;

	float m_scrollRate;			//スクロールレート
	int m_totalNotes;			//トータルノーツ数
	uint64 m_finishSample;		//終了のサンプリング位置


	String m_title;				//アーティスト名＋曲名

								//演奏開始と終了
	bool m_isStart;
	bool m_isFinish;
	EasingController<int> m_barXEasing;

	double m_nowCount;			//現在のカウント

	std::shared_ptr<IPlayBG> m_playBG;
	AudioSpectrum m_spectrum;
	void uiDraw()const;
public:

	static Effect& GetEffect(std::size_t index=0)
	{
		static std::array<Effect,2> effects;

		return effects[index];
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
		//譜面の初期化
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
