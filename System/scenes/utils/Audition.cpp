#include <scenes/utils/Audition.hpp>
#include <core/Data/MusicData/MusicData.hpp>
#include <utils/Asset/SivAssetUtil.hpp>
#include <utils/Thread/Task.hpp>
#include <Siv3D.hpp>

namespace {
	using namespace ct;

	template<class T>
	struct ScopedLoadAssetAsync
	{
		ScopedLoadAssetAsync(AssetNameView _id, s3d::HashTable<s3d::AssetName, s3d::int32>& _counter):
			id(_id),
			counter(_counter)
		{
			if (counter[id] == 0) {
				T::LoadAsync(id);
			}
			counter[id] += 1;
		}
		~ScopedLoadAssetAsync()
		{
			counter[id] -= 1;
			if (counter[id] == 0) {
				T::Release(id);
			}
		}
		AssetNameView id;
		s3d::HashTable<s3d::AssetName, s3d::int32>& counter;
	};

	constexpr size_t noSoundSample = 22050;
	Wave CreateAuditionWave(const std::stop_token& stopToken, AudioAsset asset, const MusicData::ABLoop& loop)
	{
		if (stopToken.stop_requested()) {
			return {};
		}
		size_t sampleLength = asset.samples();
		size_t loopBegin = Min(static_cast<size_t>(44100 * loop.x), sampleLength);
		size_t loopEnd = Clamp(static_cast<size_t>(44100 * loop.y), loopBegin, sampleLength);
		Array<WaveSample> wavSamples;
		wavSamples.reserve(noSoundSample + loopEnd - loopBegin);

		// 無音追加
		auto sam = WaveSample(0, 0);
		wavSamples.insert(wavSamples.begin(), noSoundSample, sam);

		if (stopToken.stop_requested()) {
			return {};
		}
		// ループサイズを追加
		const float* left = asset.getSamples(0);
		const float* right = asset.getSamples(1);
		for (size_t i = loopBegin; i < loopEnd; ++i) {
			if (stopToken.stop_requested()) {
				return {};
			}
			wavSamples.emplace_back(left[i], right[i]);
		}
		return Wave{ std::move(wavSamples) };
	}
}

namespace ct
{
	Audition::Audition() :
		m_nowPlayMusicIndex(static_cast<size_t>(-1))
	{}


	Audition::~Audition()
	{
		this->clear();
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
		const size_t id = musicData.getIndex();

		if (id == m_nowPlayMusicIndex) {
			// 同じidだった場合は何もしない
			return false;
		}
		m_nowPlayMusicIndex = id;

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
	void Audition::clear()
	{
		this->stop();
		m_loadTask.clear();
		SivAssetUtil::ReleaseByTag<AudioAsset>(U"MusicData");
	}
	Coro::Fiber<void> Audition::playAsync(s3d::uint64 requestId, const MusicData& musicData)
	{
		const String& id = musicData.getSoundNameID();
		const MusicData::ABLoop& loop = musicData.getLoopRange();
		ScopedLoadAssetAsync<AudioAsset> scopedLoad(id, m_requestCounter);
		while (!AudioAsset::IsReady(id)) {
			co_yield{};
		}
		if (requestId != m_requestId) {
			co_return;
		}
		AudioAsset audioBase(id);
		Thread::Task loadTask{ ::CreateAuditionWave, audioBase, loop };
		while (loadTask.isBusy()) {
			if (requestId != m_requestId) {
				// リクエストが変わった時点で停止
				loadTask.request_stop();
				loadTask.get();
				co_return;
			}
			co_yield{};
		}
		Audio loadedAudio(loadTask.get(), Arg::loopBegin = noSoundSample);
		if (requestId != m_requestId) {
			co_return;
		}
		m_audio = loadedAudio;
		if (!m_audio.isPlaying()) {
			m_audio.seekSamples(static_cast<size_t>(loop.x * m_audio.sampleRate()));
			m_audio.play(3s);
		}
		co_return;
	}
}
