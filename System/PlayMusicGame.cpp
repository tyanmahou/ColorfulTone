#include"PlayMusicGame.h"
#include"StartAnime.h"
#include"AutoPlayManager.h"
#include"MainScene.h"
#include"PlayKey.h"
#include"FontKinetic.h"
#include"ResultRank.h"
#include"Util.h"
#include"PlayStyle.h"

PlayMusicGame::PlayMusicGame() :
	m_nowCount(-10000.0),
	m_scrollRate(1.0f),
	m_totalNotes(0),
	m_isFinish(false),
	m_isStart(false),
	m_barXEasing(780, 730, Easing::Linear, 250),
	m_spectrum(300)
{
	StartAnime::Open();
}

void PlayMusicGame::init(MusicData & nowMusic, const int level, const float scrollRate)
{

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
	//譜面取得
	m_notesData = nowMusic.getNotesData()[level];
	//譜面の初期化
	m_notesData.init();

	m_totalNotes = m_notesData.getTotalNotes();

	m_scrollRate = scrollRate;

	Game::Instance()->m_config.m_scrollRate = m_scrollRate;

	m_title = nowMusic.getArtistName() + L" - " + nowMusic.getMusicName();

	m_score = Score();

	m_playBG = ::CreateBG(Game::Instance()->m_config.m_bgType);

	m_playBG->init(nowMusic);

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
	if (AutoPlayManager::Instance()->m_autoPlay)
		AutoPlayManager::Instance()->update();

	//ノーツ処理
	m_notesData.update(m_sound, m_nowCount, m_score);

	//コンボ更新
	m_score.m_maxCombo = Max(m_score.m_maxCombo, m_score.m_currentCombo);

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
	if (m_isCourse)
	{
		m_life = ResultRank::calcLifeRate(m_score, m_initRate);
		if (m_life <= 0)
		{
			m_isFinish = true;
		}
	}
}



namespace
{
	bool isInput(bool autoPlay, bool userPlay)
	{
		return AutoPlayManager::Instance()->m_autoPlay ? autoPlay : userPlay;
	}
}

void PlayMusicGame::drawBG(const MusicData & nowMusic, const double drawCount)const
{

	m_playBG->apply(drawCount);

	//スペクトラム描画
	if (Game::Instance()->m_config.m_isSpectrum)
		m_spectrum.draw(m_sound);

	TextureAsset(L"mainbg").draw();
}

void PlayMusicGame::draw(const MusicData & nowMusic) const
{

	const double drawCount = m_notesData.calDrawCount(m_nowCount);



	/**********/
	//背景
	this->drawBG(nowMusic, drawCount);

	//入力アクション
	const bool redInput = isInput(AutoPlayManager::Instance()->m_isRedPressed, PlayKey::Red().pressed);
	const bool blueInput = isInput(AutoPlayManager::Instance()->m_isBluePressed, PlayKey::Blue().pressed);
	const bool yellowInput = isInput(AutoPlayManager::Instance()->m_isYellowPressed, PlayKey::Yellow().pressed);


	PlayStyle::Instance()->drawFrame(redInput, blueInput, yellowInput,
		[&] {m_notesData.draw(drawCount, m_scrollRate); });

	Graphics2D::SetStencilState(StencilState::Default);


	this->uiDraw();

}

void PlayMusicGame::previewDraw(const MusicData & nowMusic, const double count) const
{
	const double drawCount = m_notesData.calDrawCount(count);

	//背景
	this->drawBG(nowMusic, drawCount);


	PlayStyle::Instance()->drawJudgeLine();

	m_notesData.previewDraw(drawCount, m_scrollRate);
}

void PlayMusicGame::uiDraw() const
{

	//UI***************************************************************

	const auto rate = m_isCourse ?
		m_life :
		Game::Instance()->m_config.m_isClearRateDownType ?
		ResultRank::calcClearRateAsDownType(m_score, m_notesData.getTotalNotes()) :
		ResultRank::calcClearRate(m_score, m_notesData.getTotalNotes());

	PlayStyle::Instance()->drawComboAndRate(m_score.m_currentCombo, rate);

	//曲の現在地
	const int barY = 50;
	{
		TextureAsset(L"streamPosBase").draw(725 - m_barXEasing.easeInOut(), barY);
		float barScale = m_sound.streamPosSample() / static_cast<float>(m_finishSample);
		if (barScale > 1.0)
			barScale = 1.0;
		const auto size = (barY + 454 - 10) - (barY + 11);

		Line({ 725 - m_barXEasing.easeInOut() + 13.5,barY + 11 + size*barScale }, { 725 - m_barXEasing.easeInOut() + 13.5,barY + 11 + 10 + size*barScale }).draw(3, Palette::Orange);

	}
	{
		TextureAsset(L"barBase").draw(m_barXEasing.easeInOut(), barY);
		float barScale = rate / 100.f;
		const auto h = TextureAsset(L"bar2").height;
		TextureAsset(L"bar2").uv(0, 1 - barScale, 1, barScale).draw(m_barXEasing.easeInOut(), barY + h*(1 - barScale));
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

	if (AutoPlayManager::Instance()->m_autoPlay)
		PutText(L"AutoPlay").at(Window::Center().x, 40);

	const auto levelName = Format(m_notesData.getLevel()) + L" - " + m_notesData.getLevelName();
	PutText(levelName).at(Window::Center().x, Window::Height() - 20);

}


