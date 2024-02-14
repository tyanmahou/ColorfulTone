#pragma once
#include <core/Data/NotesData/NotesData.hpp>
#include <core/Play/PlayNotesData.hpp>
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
	public:
		PlayMusicGame();

		void init(const NotesData& notes, const double scrollRate);
		void finally();

		void reflesh();
		void reflesh(const NotesData& notes);

		void synchroCount(double& count);

		void update();

		void drawBG(const double drawCount)const;

		void draw(bool preview = false)const;

		void playModeDraw()const;
		void previewDraw(const double count)const;

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
		void notesInit();

		const PlayNotesData& getPlayNotesData() const;

		void setCourseMode(const Score& score);
		static void ScoreUpdate(Score::Judge judge, NoteType type, NoteType baseType, bool playSe);
	private:
		void uiDraw(bool preview)const;
		void drawCurrentBPM()const;
		void drawAutoPlay(bool preview) const;
		void drawRandomMode() const;
		void drawMusicTitle(bool preview) const;
		void drawNotesLevel() const;

	private:
		PlayNotesData m_playNotesData;

		bool m_isCourse = false;

		s3d::Audio m_sound;				// 再生する曲
		s3d::String m_soundNameID;

		ApAnime m_FCAPAnime;	        // フルコンAPアニメ
		Score m_score;

		double m_scrollRate;			// スクロールレート
		s3d::uint32 m_totalNotes;		// トータルノーツ数
		s3d::uint64 m_finishSample;		// 終了のサンプリング位置
		double m_soundLengthSec = 0;    // 曲の長さ

		s3d::String m_title;			// アーティスト名＋曲名

		//演奏開始と終了
		bool m_isStart;
		bool m_isFinish;
		s3d::EasingAB<s3d::int32> m_barXEasing;

		double m_nowCount;			    // 現在のカウント

		std::shared_ptr<IPlayBG> m_playBG;
		AudioSpectrum m_spectrum;
    };
}