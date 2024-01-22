﻿#include <core/Play/PlayMusicGame.hpp>
#include <Useful.hpp>
#include <core/Anime/StartAnime/StartAnime.hpp>
#include <core/Input/InputManager.hpp>
#include <core/Input/AutoPlayManager.hpp>
#include <core/Data/Score/ResultRank.hpp>
#include <core/Play/PlayBG/PlayBGFactory.hpp>
#include <core/Play/PlayStyle/PlayStyle.hpp>
#include <Siv3D.hpp>
#include "PlayMusicGame.hpp"

namespace
{
	using namespace ct;
	Score* g_pScore;
	Audio* g_pSound;

	static const std::unordered_map<Score::Judge, String> scoreMap
	{
		{ Score::Good,U"GOOD" },
		{ Score::Great,U"GREAT" },
		{ Score::Perfect,U"PERFECT" },
		{ Score::Miss, U"MISS" },
	};

	void HandleAddJudgeEffect(Score::Judge judge, NoteType type)
	{
		const String judgeName = scoreMap.at(judge);

		PlayStyle::Instance()->drawJudgeEffect(judgeName, type);
		if (judge != Score::Miss) {
			PlayStyle::Instance()->drawTapEffect(type);
		}
	}

	bool isInput(bool autoPlay, bool userPlay)
	{
		return AutoPlayManager::IsAutoPlay() ? autoPlay : userPlay;
	}

	Stopwatch g_startTimer;
}
namespace ct
{
	PlayMusicGame::PlayMusicGame() :
		m_nowCount(-10000.0),
		m_scrollRate(1.0f),
		m_totalNotes(0),
		m_isFinish(false),
		m_isStart(false),
		m_barXEasing(780, 730, Easing::Linear, 250),
		m_spectrum(300)
	{
		// プレイ中のスコアを参照しておく
		g_pScore = &m_score;
		g_pSound = &m_sound;
	}

	void PlayMusicGame::init(const NotesData& notes, const float scrollRate)
	{
		//譜面取得
		m_notesData = notes;
		//譜面の初期化
		m_notesData.init();

		const MusicData& nowMusic = *notes.getMusic();

		m_soundNameID = nowMusic.getSoundNameID();

		AudioAsset::Load(m_soundNameID);
		//wav取得
		Audio sound = nowMusic.getSound();
		Wave wav = [](const Audio& sound)->Wave {
			Array<WaveSample> wavSamples;
			size_t sampleLength = sound.samples();
			wavSamples.reserve(sampleLength);

			const float* left = sound.getSamples(0);
			const float* right = sound.getSamples(1);
			for (size_t i = 0; i < sampleLength; ++i) {
				wavSamples.emplace_back((*left), (*right));
				++left;
				++right;
			}
			return Wave{ std::move(wavSamples) };
		}(sound);
		AudioAsset::Release(m_soundNameID);// .release();

		//新wavのサンプリング数
		const size_t beginOffset = 44100 * 4 + 735 * Game::Config().m_timingAdjust;
		const size_t endOffset = 44100 * 4;
		const size_t sample = beginOffset + wav.lengthSample() + endOffset;

		//無音作成
		auto sam = WaveSample(0, 0);
		wav.reserve(sample);
		//wavに4秒間のオフセット追加
		wav.insert(wav.begin(), beginOffset, sam);
		m_finishSample = wav.lengthSample();
		wav.insert(wav.end(), endOffset, sam);
		m_sound = Audio(wav);

		m_totalNotes = m_notesData.getTotalNotes();

		m_scrollRate = scrollRate;

		Game::Config().m_scrollRate = m_scrollRate;

		m_title = nowMusic.getArtistName() + U" - " + nowMusic.getMusicName();

		m_score = Score();

		m_playBG = PlayBGFactory::CreateBG(Game::Config().m_bgType);
		PlayStyle::Instance()->setStyle(Game::Config().m_styleType);

		m_playBG->init(notes);
	}

	void PlayMusicGame::finally()
	{
		m_sound.stop(1s);
	}

	void PlayMusicGame::update()
	{
		if (!m_isStart) {
			SoundManager::PlayInGameMusic(m_sound);
			m_barXEasing.start();
			m_isStart = true;
			g_startTimer.restart();
			return;
		}
		//オートプレイのキー入力更新
		if (AutoPlayManager::IsAutoPlay()) {
			AutoPlayManager::Update();
		} else {
			InputManager::Update();
		}
		//ノーツ処理
		m_notesData.update(m_sound, m_nowCount, m_score);

		//フルコン演出
		if (m_score.m_maxCombo >= m_totalNotes && !m_FCAPAnime.isStart()) {
			m_FCAPAnime.play(m_score);
		}
		if (m_FCAPAnime.isStart())
			m_FCAPAnime.update();

		const int64 sample = GetSamplePos(m_sound);

		//曲の終わり
		if (!m_isFinish) {
			if (static_cast<uint32>(sample) >= m_finishSample || m_nowCount >= m_notesData.getMaxBarCount()) {
				m_isFinish = true;
				this->stopSound();
			}
		}
		if (m_isCourse && m_score.m_life <= 0) {
			m_isFinish = true;
		}
	}

	void PlayMusicGame::setCourseMode(const Score& score)
	{
		m_isCourse = true;
		m_score.m_lifeHistory[0] = score.m_life;
		m_score.m_initLife = score.m_life;
		m_score.m_life = score.m_life;
	}


	void PlayMusicGame::ScoreUpdate(Score::Judge judge, NoteType type, bool playSe)
	{
		if (!g_pScore) {
			Print << U"Warning: Missing Score";
		}

		g_pScore->add(judge);
		::HandleAddJudgeEffect(judge, type);

		if (playSe && judge != Score::Miss) {
			SoundManager::PlayInGameSe(scoreMap.at(judge));
		}
	}

	const Audio* const PlayMusicGame::CurrentSound()
	{
		return g_pSound;
	}
	void PlayMusicGame::drawBG(const double drawCount)const
	{

		m_playBG->apply(drawCount);

		//スペクトラム描画
		if (Game::Config().m_isSpectrum) {
			m_spectrum.draw(m_sound);
		}
		TextureAsset(U"mainbg").draw();
	}

	void PlayMusicGame::draw() const
	{

		const double drawCount = m_notesData.calDrawCount(m_nowCount);

		/**********/
		//背景
		this->drawBG(drawCount);

		{
			int beat = NotesData::RESOLUTION / 4;
			double f = Abs(static_cast<double>(static_cast<int>(m_nowCount) % beat))
				/ static_cast<double>(beat);

			constexpr int w = 80;
			ColorF c1 = ColorF(0, 0, 0, 0.6 * (1 - f));
			ColorF c2 = m_notesData.getColor();
			c2.setA(0);

			Rect(0, 0, w, 600).draw({ c1,c2, c2,c1 });
			Rect(800, 0, -w, 600).draw({ c1,c2, c2,c1 });
		}
		//入力アクション
		const bool redInput = isInput(AutoPlayManager::IsRedPressed(), PlayKey::Red().pressed());
		const bool blueInput = isInput(AutoPlayManager::IsBluePressed(), PlayKey::Blue().pressed());
		const bool yellowInput = isInput(AutoPlayManager::IsYellowPressed(), PlayKey::Yellow().pressed());


		PlayStyle::Instance()->drawFrame(redInput, blueInput, yellowInput,
			[&] {
				PlayMusicGame::GetEffect().update();
				m_notesData.draw(drawCount, m_scrollRate);
			});

		this->uiDraw();

		if (g_startTimer.ms() <= 3000) {
			StartAnime::Draw((g_startTimer.ms() - 1000) / 2000.0);
		}
	}

	void PlayMusicGame::previewDraw(const double count) const
	{
		const double drawCount = m_notesData.calDrawCount(count);

		//背景
		this->drawBG(drawCount);

		PlayStyle::Instance()->drawJudgeLine();

		m_notesData.previewDraw(drawCount, m_scrollRate);

		PutText(Format(U"length:", m_notesData.getMusic()->getLengthSec()), Arg::topLeft = Vec2{ 20, Scene::Height() - 120});
		PutText(Format(U"total:", m_totalNotes), Arg::topLeft = Vec2{ 20, Scene::Height() - 140 });
	}

	void PlayMusicGame::drawCurrentBPM() const
	{
		BPMType bpm = m_notesData.getCurrentBPM();
		s3d::String tmp = U"{:.2f}*{:.1f}={:.2f}"_fmt(bpm, m_scrollRate, bpm * static_cast<double>(m_scrollRate));
		PutText(tmp, { 20, Scene::Height() - 100 });
	}

	bool PlayMusicGame::isFinish() const
	{
		return m_isFinish && (m_FCAPAnime.isEnd() || !m_FCAPAnime.isStart());
	}

	void PlayMusicGame::uiDraw() const
	{

		//UI***************************************************************

		const IndicateRate rateType = Game::Config().m_rateType;
		const auto rate = m_isCourse || rateType == IndicateRate::Life ?
			ResultRank::CalcLifeRate(m_score) :
			rateType == IndicateRate::Down ?
			ResultRank::CalcClearRateAsDownType(m_score, m_notesData.getTotalNotes()) :
			ResultRank::CalcClearRate(m_score, m_notesData.getTotalNotes());

		PlayStyle::Instance()->drawComboAndRate(m_score.m_currentCombo, rate);

		//曲の現在地
		const int barY = 50;
		{
			TextureAsset(U"streamPosBase").draw(725 - m_barXEasing.easeInOut(), barY);
			float barScale = GetSamplePos(m_sound) / static_cast<float>(m_finishSample);
			if (barScale > 1.0)
				barScale = 1.0;
			const auto size = (barY + 454 - 10) - (barY + 11);

			Line({ 725 - m_barXEasing.easeInOut() + 13.5,barY + 11 + size * barScale }, { 725 - m_barXEasing.easeInOut() + 13.5,barY + 11 + 10 + size * barScale }).draw(3, Palette::Orange);

		}
		{
			TextureAsset(U"barBase").draw(m_barXEasing.easeInOut(), barY);
			float barScale = rate / 100.f;
			const auto h = TextureAsset(U"bar2").height();
			TextureAsset(U"bar2").uv(0, 1 - barScale, 1, barScale).draw(m_barXEasing.easeInOut(), barY + h * (1 - barScale));
			TextureAsset(U"bar1").draw(m_barXEasing.easeInOut(), barY);
		}

		//FCアニメ
		if (m_FCAPAnime.isStart()) {
			if (!m_FCAPAnime.isEnd()) {
				m_FCAPAnime.draw();
			}
		}
		PutText(m_title, { Scene::Center().x, 20 });

		if (AutoPlayManager::IsAutoPlay())
			PutText(U"AutoPlay", { Scene::Center().x, 40 });

		const auto levelName = m_notesData.getLevelWithStar() + U" - " + m_notesData.getLevelName();
		PutText(levelName, Vec2{ Scene::Center().x, Scene::Height() - 20 });
	}
}