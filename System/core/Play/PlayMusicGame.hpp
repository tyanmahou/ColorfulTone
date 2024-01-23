#pragma once
#include <core/Data/NotesData/NotesData.hpp>
#include <core/Data/Score/Score.hpp>
#include <utils/Audio/AudioSpectrum.hpp>
#include <core/Anime/ApAnime/ApAnime.hpp>
#include <Siv3D/Audio.hpp>
#include <Siv3D/Effect.hpp>
#include <Siv3D/EasingAB.hpp>

namespace ct
{
	class IPlayBG;

    class PlayMusicGame
    {
	private:

		bool m_isCourse = false;

		s3d::Audio m_sound;				//再生する曲
		s3d::String m_soundNameID;
		NotesData m_notesData;		//譜面情報

		ApAnime m_FCAPAnime;	//フルコンAPアニメ
		Score m_score;

		double m_scrollRate;			//スクロールレート
		s3d::uint32 m_totalNotes;			//トータルノーツ数
		s3d::uint64 m_finishSample;		//終了のサンプリング位置


		s3d::String m_title;				//アーティスト名＋曲名

		//演奏開始と終了
		bool m_isStart;
		bool m_isFinish;
		s3d::EasingAB<int> m_barXEasing;

		double m_nowCount;			//現在のカウント

		std::shared_ptr<IPlayBG> m_playBG;
		AudioSpectrum m_spectrum;
		void uiDraw()const;
	public:

		static s3d::Effect& GetEffect(std::size_t index = 0)
		{
			static std::array<s3d::Effect, 2> effects;

			return effects[index];
		}

		PlayMusicGame();

		void init(const NotesData& notes, const float scrollRate);
		void finally();

		void synchroCount(double& count)
		{
			m_notesData.synchroCount(m_sound, count);
		}
		void update();

		void drawBG(const double drawCount)const;

		void draw()const;

		void previewDraw(const double count)const;

		void drawCurrentBPM()const;

		bool isFinish()const;

		inline const Score& getScore()const
		{
			return m_score;
		}
		inline const Audio& getSound()const
		{
			return m_sound;
		}

		inline void stopSound(const s3d::SecondsF& fadeoutDuration = s3d::SecondsF(2.0))
		{
			m_sound.stop(fadeoutDuration);
		}
		inline void setScrollRate(double rate)
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

		void setCourseMode(const Score& score);
		static void ScoreUpdate(Score::Judge judge, NoteType type, bool playSe = true);
        static const s3d::Audio* const CurrentSound();
    };
}