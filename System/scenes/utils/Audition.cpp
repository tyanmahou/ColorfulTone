#include <scenes/utils/Audition.hpp>
#include <core/Data/MusicData/MusicData.hpp>
#include <utils/Asset/SivAssetUtil.hpp>
#include <utils/Thread/Task.hpp>
#include <Siv3D.hpp>
#include "Audition.hpp"

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
		if (stopToken.stop_requested()) {
			return {};
		}
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
		wav.insert(wav.begin() + static_cast<size_t>(44100 * loop.x), 22050, sam);
		return Audio(std::move(wav), static_cast<uint64>(44100 * (loop.x + 0.5)), static_cast<uint64>(44100 * (loop.y + 0.5)));
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
		ScopedLoadAssetAsync<AudioAsset> scopedLoad(id);
		while (!AudioAsset::IsReady(id)) {
			// 他のリクエストでキャンセルされる可能性があるので毎回ロードリクエストはしておく
			AudioAsset::LoadAsync(id);
			co_yield{};
		}
		if (requestId != m_requestId) {
			co_return;
		}
		Thread::Task loadTask{ ::CreateAuditionSound, id, loop };
		while (loadTask.isBusy()) {
			if (requestId != m_requestId) {
				// リクエストが変わった時点で停止
				loadTask.request_stop();
				loadTask.get();
				co_return;
			}
			co_yield{};
		}
		Audio loadedAudio = loadTask.get();
		if (!loadedAudio) {
			co_return;
		}
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
