#pragma once
#include<Siv3D.hpp>

class MusicData;

class Audition
{
private:
	int m_nowPlayMusicIndex;
public:
	Audition();
	~Audition();

	bool autoPlayAndStop(const MusicData& musicData);

	void play(const MusicData& musicData);
	void stop()const;
};

