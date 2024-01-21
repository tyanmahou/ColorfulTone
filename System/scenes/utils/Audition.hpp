#pragma once
#include <Siv3D/Types.hpp>

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
		void stop()const;

	private:
		s3d::int32 m_nowPlayMusicIndex;
	};

}
