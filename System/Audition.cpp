#include "Audition.hpp"
#include "MusicData.h"
#include "MultiThread.hpp"
#include "SoundManager.h"

namespace {

	Sound g_sampleSound;

	Sound createAuditionSound(const String& id, const MusicData::ABLoop loop)
	{
		Wave wav = SoundAsset(id).getWave();

		const size_t sample = 22050 + wav.lengthSample;

		//無音作成
		auto sam = WaveSample(0, 0);
		wav.reserve(sample);
		//wavに4秒間のオフセット追加
		wav.insert(wav.begin() + 44100 * loop.x, 22050, sam);

		SoundAsset::Release(id);
		return Sound(wav);
	}

	void AudioPlay(const MusicData & musicData)
	{
		const String& id = musicData.getSoundNameID();
		const MusicData::ABLoop& loop = musicData.getLoopRange();
		g_sampleSound = ::createAuditionSound(id, loop);
		g_sampleSound.setVolume(SoundManager::BGM::GetVolume());
		if (!g_sampleSound.isPlaying())
		{
			g_sampleSound.setLoopBySec(true, loop.x + 0.5, loop.y + 0.5);
			g_sampleSound.setPosSec(loop.x);
			g_sampleSound.play(3s);
		}
	}
}


Audition::Audition():
	m_nowPlayMusicIndex(-1)
{}


Audition::~Audition()
{
	this->stop();
	SoundAsset::ReleaseByTag(L"MusicData");
	g_sampleSound.release();
}

bool Audition::autoPlayAndStop(const MusicData & musicData)
{
	const int id = musicData.getIndex();

	if (id == m_nowPlayMusicIndex)
	{
		// 同じidだった場合は何もしない
		return false;
	}
	this->stop();
	this->play(musicData);
	return true;
}

void Audition::play(const MusicData & musicData)
{
	m_nowPlayMusicIndex = musicData.getIndex();

	MultiThread::Async(L"audio_play", AudioPlay,musicData);
}

void Audition::stop() const
{
	MultiThread::Wait(L"audio_play");
	g_sampleSound.stop(1s);
}
