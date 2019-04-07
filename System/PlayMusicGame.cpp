#include"PlayMusicGame.h"
#include"Useful.hpp"
#include"StartAnime.h"
#include"InputManager.hpp"
#include"AutoPlayManager.h"
#include"MainScene.h"
#include"FontKinetic.h"
#include"ResultRank.h"
#include"PlayStyle.h"

namespace
{
	Score* g_pScore;
	Sound* g_pSound;

	static const std::unordered_map<Score::Judge, String> scoreMap
	{
		{ Score::Good,L"GOOD" },
		{ Score::Great,L"GREAT" },
		{ Score::Perfect,L"PERFECT" },
		{ Score::Miss, L"MISS" },
	};

	void HandleAddJudgeEffect(Score::Judge judge, NoteType type)
	{
		const String judgeName = scoreMap.at(judge);

		PlayStyle::Instance()->drawJudgeEffect(judgeName, type);
		if (judge != Score::Miss)
		{
			PlayStyle::Instance()->drawTapEffect(type);
		}
	}
}
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

	StartAnime::Open();
}

void PlayMusicGame::init(const NotesData& notes, const float scrollRate)
{
	//譜面取得
	m_notesData = notes;
	//譜面の初期化
	m_notesData.init();

	const MusicData &nowMusic = *notes.getMusic();

	m_soundNameID = nowMusic.getSoundNameID();

	SoundAsset::Preload(m_soundNameID);
	//wav取得
	Wave wav = nowMusic.getSound().getWave();

	SoundAsset::Release(m_soundNameID);// .release();

									   //新wavのサンプリング数
	const size_t sample = 44100 * 8 + wav.lengthSample;

	//無音作成
	auto sam = WaveSample(0, 0);
	wav.reserve(sample);
	//wavに4秒間のオフセット追加
	wav.insert(wav.begin(), 44100 * 4, sam);
	m_finishSample = wav.lengthSample;
	wav.insert(wav.end(), 44100 * 4, sam);
	m_sound = Sound(wav);
	m_sound.setVolume(SoundManager::BGM::GetVolume());

	m_totalNotes = m_notesData.getTotalNotes();

	m_scrollRate = scrollRate;

	Game::Config().m_scrollRate = m_scrollRate;

	m_title = nowMusic.getArtistName() + L" - " + nowMusic.getMusicName();

	m_score = Score();

	m_playBG = ::CreateBG(Game::Config().m_bgType);
	PlayStyle::Instance()->setStyle(Game::Config().m_styleType);

	m_playBG->init(notes);
}

void PlayMusicGame::finally()
{
	m_sound.stop(1s);
}

void PlayMusicGame::update()
{
	if (!m_isStart)
	{
		m_sound.play();
		StartAnime::Play();
		m_barXEasing.start();
		m_isStart = true;
		return;
	}
	//オートプレイのキー入力更新
	if (AutoPlayManager::IsAutoPlay())
	{
		AutoPlayManager::Update();
	}
	else {
		InputManager::Update();
	}
	//ノーツ処理
	m_notesData.update(m_sound, m_nowCount, m_score);

	//フルコン演出
	if (m_score.m_maxCombo >= m_totalNotes && !m_FCAPAnime.isStart())
	{
		m_FCAPAnime.play(m_score);
	}
	if (m_FCAPAnime.isStart())
		m_FCAPAnime.update();

#ifdef VIEWER_MODE
	const int64 sample = m_sound.streamPosSample();
#else
	const int64 sample = m_sound.samplesPlayed();

#endif

	//曲の終わり
	if (!m_isFinish)
	{
		if (sample >= m_finishSample || m_nowCount >= m_notesData.getMaxBarCount())
		{
			m_isFinish = true;
			this->stopSound();
		}
	}
	if (m_isCourse && m_score.m_life <= 0)
	{
		m_isFinish = true;
	}
}

void PlayMusicGame::setCourseMode(const Score & score)
{
	m_isCourse = true;
	m_score.m_lifeHistory[0] = score.m_life;
	m_score.m_initLife = score.m_life;
	m_score.m_life = score.m_life;
}


void PlayMusicGame::ScoreUpdate(Score::Judge judge, NoteType type, bool playSe)
{
	if (!g_pScore)
	{
		Println(L"Warning: Missing Score");
	}

	g_pScore->add(judge);
	::HandleAddJudgeEffect(judge, type);

	if (playSe && judge != Score::Miss)
	{
		SoundManager::SE::Play(scoreMap.at(judge));
	}
}

const Sound*const PlayMusicGame::CurrentSound()
{
	return g_pSound;
}


namespace
{
	bool isInput(bool autoPlay, bool userPlay)
	{
		return AutoPlayManager::IsAutoPlay() ? autoPlay : userPlay;
	}
}

void PlayMusicGame::drawBG(const double drawCount)const
{

	m_playBG->apply(drawCount);

	//スペクトラム描画
	if (Game::Config().m_isSpectrum)
	{
		m_spectrum.draw(m_sound);
	}
	TextureAsset(L"mainbg").draw();
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
		ColorF c1 = ColorF(0, 0, 0, 0.6*(1 - f));
		ColorF c2 = m_notesData.getColor();
		c2.setAlpha(0);

		Rect(0, 0, w, 600).draw({ c1,c2, c2,c1 });
		Rect(800, 0, -w, 600).draw({ c1,c2, c2,c1 });
	}
	//入力アクション
	const bool redInput = isInput(AutoPlayManager::IsRedPressed(), PlayKey::Red().pressed);
	const bool blueInput = isInput(AutoPlayManager::IsBluePressed(), PlayKey::Blue().pressed);
	const bool yellowInput = isInput(AutoPlayManager::IsYellowPressed(), PlayKey::Yellow().pressed);


	PlayStyle::Instance()->drawFrame(redInput, blueInput, yellowInput,
		[&] {
		PlayMusicGame::GetEffect().update();
		m_notesData.draw(drawCount, m_scrollRate);
	});

	Graphics2D::SetStencilState(StencilState::Default);


	this->uiDraw();

}

void PlayMusicGame::previewDraw(const double count) const
{
	const double drawCount = m_notesData.calDrawCount(count);

	//背景
	this->drawBG(drawCount);

	PlayStyle::Instance()->drawJudgeLine();

	m_notesData.previewDraw(drawCount, m_scrollRate);

	PutText(Format(L"length:",m_notesData.getMusic()->getLengthSec())).from(20, Window::Height() - 120);
	PutText(Format(L"total:",m_totalNotes)).from(20, Window::Height() - 140);
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
		ResultRank::CalcLifeRate(m_score):
		rateType == IndicateRate::Down ?
		ResultRank::CalcClearRateAsDownType(m_score, m_notesData.getTotalNotes()) :
		ResultRank::CalcClearRate(m_score, m_notesData.getTotalNotes());

	PlayStyle::Instance()->drawComboAndRate(m_score.m_currentCombo, rate);

	//曲の現在地
	const int barY = 50;
	{
		TextureAsset(L"streamPosBase").draw(725 - m_barXEasing.easeInOut(), barY);
		float barScale = m_sound.streamPosSample() / static_cast<float>(m_finishSample);
		if (barScale > 1.0)
			barScale = 1.0;
		const auto size = (barY + 454 - 10) - (barY + 11);

		Line({ 725 - m_barXEasing.easeInOut() + 13.5,barY + 11 + size * barScale }, { 725 - m_barXEasing.easeInOut() + 13.5,barY + 11 + 10 + size * barScale }).draw(3, Palette::Orange);

	}
	{
		TextureAsset(L"barBase").draw(m_barXEasing.easeInOut(), barY);
		float barScale = rate / 100.f;
		const auto h = TextureAsset(L"bar2").height;
		TextureAsset(L"bar2").uv(0, 1 - barScale, 1, barScale).draw(m_barXEasing.easeInOut(), barY + h * (1 - barScale));
		TextureAsset(L"bar1").draw(m_barXEasing.easeInOut(), barY);
	}

	//スタートアニメ
	StartAnime::Draw();

	//FCアニメ
	if (m_FCAPAnime.isStart())
	{
		if (!m_FCAPAnime.isEnd())
		{
			m_FCAPAnime.draw();
		}
	}

	PutText(m_title).at(Window::Center().x, 20);

	if (AutoPlayManager::IsAutoPlay())
		PutText(L"AutoPlay").at(Window::Center().x, 40);

	const auto levelName = Format(m_notesData.getLevel()) + L" - " + m_notesData.getLevelName();
	PutText(levelName).at(Window::Center().x, Window::Height() - 20);
}


