#pragma once
#include <Siv3D/Types.hpp>
#include <utils/Thread/Task.hpp>

namespace ct
{
	class MusicData;

	class Audition
	{
	public:
		Audition();
		~Audition();

		bool autoPlayAndStop(const MusicData& musicData);

		void play(const MusicData& musicData);
		void stop();
	private:
		void playInternal(const std::stop_token& stopToken, const MusicData& musicData);
	private:
		s3d::int32 m_nowPlayMusicIndex;
		Audio m_audio;
		std::unique_ptr<Thread::Task<void>> m_loadTask;
	};

}
