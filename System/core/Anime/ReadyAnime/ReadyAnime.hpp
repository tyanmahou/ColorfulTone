#pragma once
#include <utils/TimeLite/Timer.hpp>

namespace ct
{
	/// <summary>
	/// Readyアニメ
	/// </summary>
	class ReadyAnime
	{
	public:
		ReadyAnime();
		~ReadyAnime();

		void playIn();
		void playOut();

		bool isPlaying() const;
		bool isEnd() const;

		void update(double dt);
		void draw() const;
	private:		
		TimeLite::Timer m_fadeTimer;
		bool m_isPlayIn = true;
	};
}