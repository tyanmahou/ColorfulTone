#include <scenes/utils/Audition.hpp>
#include <core/Data/MusicData/MusicData.hpp>
#include <utils/Asset/SivAssetUtil.hpp>
#include <Siv3D.hpp>

// TODO 調整
namespace {
	using namespace ct;

	template<class T>
	struct ScopedAsset
	{
		ScopedAsset(AssetNameView _id):
			id(_id)
		{
			T::Load(id);
		}
		~ScopedAsset()
		{
			T::Release(id);
		}
		AssetNameView id;
	};
	Audio CreateAuditionSound(const std::stop_token& stopToken, AssetNameView id, const MusicData::ABLoop& loop)
	{
		ScopedAsset<AudioAsset> scopedAsset(id);
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
		this->stop();
		SivAssetUtil::ReleaseByTag<AudioAsset>(U"MusicData");
		m_audio.release();
	}

	bool Audition::autoPlayAndStop(const MusicData& musicData)
	{
		const int32 id = musicData.getIndex();

		if (id == m_nowPlayMusicIndex) {
			// 同じidだった場合は何もしない
			return false;
		}
		this->stop();
		this->play(musicData);
		return true;
	}

	void Audition::play(const MusicData& musicData)
	{
		m_nowPlayMusicIndex = musicData.getIndex();

		m_loadTask = std::make_unique<Thread::Task<void>>([&](const std::stop_token& stopToken){
			this->playInternal(stopToken, musicData);
		});
	}

	void Audition::stop()
	{
		m_loadTask = nullptr;
		m_audio.stop(1s);
	}
	void Audition::playInternal(const std::stop_token& stopToken, const MusicData& musicData)
	{
		const String& id = musicData.getSoundNameID();
		const MusicData::ABLoop& loop = musicData.getLoopRange();
		m_audio = ::CreateAuditionSound(stopToken, id, loop);
		if (!m_audio.isPlaying()) {
			m_audio.seekSamples(loop.x * m_audio.sampleRate());
			m_audio.play(3s);
		}
	}
}
