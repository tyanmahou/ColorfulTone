﻿#include <scenes/utils/Audition.hpp>
#include <core/Data/MusicData/MusicData.hpp>
#include <utils/Asset/SivAssetUtil.hpp>
#include <utils/Thread/Task.hpp>
#include <Siv3D.hpp>

// TODO 調整
namespace {
	using namespace ct;

	template<class T>
	struct ScopedLoadAssetAsync
	{
		ScopedLoadAssetAsync(AssetNameView _id):
			id(_id)
		{
			T::LoadAsync(id);
		}
		~ScopedLoadAssetAsync()
		{
			T::Release(id);
		}
		AssetNameView id;
	};
	Audio CreateAuditionSound(const std::stop_token& stopToken, AssetNameView id, const MusicData::ABLoop& loop)
	{
		AudioAsset asset(id);
		Wave wav = [&stopToken](const Audio& sound)->Wave {
			Array<WaveSample> wavSamples;
			size_t sampleLength = sound.samples();
			wavSamples.reserve(sampleLength);

			const float* left = sound.getSamples(0);
			const float* right = sound.getSamples(1);
			for (size_t i = 0; i < sampleLength; ++i) {
				if (stopToken.stop_requested()) {
					return {};
				}
				wavSamples.emplace_back((*left), (*right));
				++left;
				++right;
			}
			return Wave{ std::move(wavSamples) };
		}(asset);
		if (stopToken.stop_requested()) {
			return {};
		}
		const size_t sample = 22050 + wav.lengthSample();

		//無音作成
		auto sam = WaveSample(0, 0);
		wav.reserve(sample);
		//wavに4秒間のオフセット追加
		wav.insert(wav.begin() + 44100 * loop.x, 22050, sam);
		return Audio(std::move(wav), 44100 * (loop.x + 0.5), 44100 * (loop.y + 0.5));
	}
}

namespace ct
{
	Audition::Audition() :
		m_nowPlayMusicIndex(-1)
	{}


	Audition::~Audition()
	{
		m_loadTask.clear();
		SivAssetUtil::ReleaseByTag<AudioAsset>(U"MusicData");
		m_audio.release();
	}

	void Audition::update()
	{
		for (auto&& task : m_loadTask) {
			task.resume();
		}
		m_loadTask.remove_if([](const Coro::FiberHolder<void>& task) {
			return task.isDone();
		});
	}

	bool Audition::request(const MusicData& musicData)
	{
		const int32 id = musicData.getIndex();

		if (id == m_nowPlayMusicIndex) {
			// 同じidだった場合は何もしない
			return false;
		}
		m_nowPlayMusicIndex = musicData.getIndex();

		stop();

		m_loadTask.push_back({});
		m_loadTask.back().reset(std::bind(&Audition::playAsync, this, m_requestId, musicData));
		return true;
	}

	void Audition::stop()
	{
		++m_requestId;
		m_audio.stop(1s);
	}
	Coro::Fiber<void> Audition::playAsync(s3d::uint64 requestId, const MusicData& musicData)
	{
		const String& id = musicData.getSoundNameID();
		const MusicData::ABLoop& loop = musicData.getLoopRange();
		ScopedLoadAssetAsync<AudioAsset> scopedLoad(id);
		while (!AudioAsset::IsReady(id)) {
			co_yield{};
		}
		if (requestId != m_requestId) {
			co_return;
		}
		Audio loadedAudio = co_await Thread::Task{ ::CreateAuditionSound , id,loop };
		if (!loadedAudio) {
			co_return;
		}
		if (requestId != m_requestId) {
			co_return;
		}
		m_audio = loadedAudio;
		if (!m_audio.isPlaying()) {
			m_audio.seekSamples(loop.x * m_audio.sampleRate());
			m_audio.play(3s);
		}
		co_return;
	}
}
