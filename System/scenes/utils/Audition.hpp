#pragma once
#include <Siv3D/Types.hpp>
#include <utils/Coro/Fiber/FiberHolder.hpp>
#include <utils/Thread/Task.hpp>

namespace ct
{
	class MusicData;

	class Audition
	{
	public:
		Audition();
		~Audition();

		void update();
		bool request(const MusicData& musicData);
		void stop();

		void clear();
	private:
		Coro::Fiber<void> playAsync(s3d::uint64 requestId, const MusicData& musicData);
	private:
		size_t m_nowPlayMusicIndex;
		Audio m_audio;
		s3d::Array<Coro::FiberHolder<void>> m_loadTask;
		s3d::uint64 m_requestId = 0;
	};

}
