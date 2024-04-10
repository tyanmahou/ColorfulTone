#include <core/Play/PlayMusicGame.hpp>
#include <Useful.hpp>
#include <core/Anime/StartAnime/StartAnime.hpp>
#include <core/Input/InputManager.hpp>
#include <core/Data/Score/ResultRank.hpp>
#include <core/Play/PlayBG/PlayBGFactory.hpp>
#include <core/Play/PlayStyle/PlayStyle.hpp>
#include <core/Play/Random/RandomNote.hpp>
#include <core/Play/ColorFx/ColorFx.hpp>
#include <core/Play/UI/GaugeView.hpp>
#include <core/Play/UI/SamplePosView.hpp>
#include <utils/Coro/Fiber/FiberUtil.hpp>
#include <Siv3D.hpp>

namespace
{
    using namespace ct;
    Score* g_pScore;

    void HandleAddJudgeEffect(Score::Judge judge, int64 diff, NoteType type, NoteType baseType)
    {
        PlayStyle::Instance()->drawJudgeEffect(judge, type, diff);
        if (judge != Score::Miss) {
            PlayStyle::Instance()->drawTapEffect(type, baseType);
        }
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
        m_isDead(false),
        m_barXEasing(0, 1.0, Easing::Linear, 250),
        m_spectrum(300),
        m_postProcessTex(Scene::Size()),
        m_usePostProcess(false)
    {
        // プレイ中のスコアを参照しておく
        g_pScore = &m_score;
    }

    void PlayMusicGame::init(const NotesData& notes, const double scrollRate)
    {
        const MusicData nowMusic = notes.getMusic();
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

        m_soundLengthSec = wav.lengthSample() / 44100.0;

        //新wavのサンプリング数
        const size_t beginOffset = 44100 * 4;
        const size_t endOffset = 44100 * 4;
        const size_t sample = beginOffset + wav.lengthSample() + endOffset;

        //無音作成
        auto sam = WaveSample(0, 0);
        wav.reserve(sample);
        //wavに4秒間のオフセット追加
        wav.insert(wav.begin(), beginOffset, sam);
        m_audioEndSample = wav.lengthSample();
        wav.insert(wav.end(), endOffset, sam);
        m_sound = Audio(wav);

        m_scrollRate = scrollRate;
        Game::Config().m_scrollRate = m_scrollRate;

        this->reflesh(notes);
    }

    void PlayMusicGame::finally()
    {
        m_sound.stop(1s);
    }

    void PlayMusicGame::reflesh()
    {    
        // ランダム初期化
        RandomNote::Init(Game::Config().m_random);
        //譜面の初期化
        m_playNotesData.reset();

        m_score = Score(Game::Config().m_lifeGauge);

        m_playBG = PlayBGFactory::CreateBG(Game::Config().m_bgType);
        PlayStyle::Instance()->setStyle(Game::Config().m_styleType);

        m_playBG->init(m_playNotesData.getNotesData());
    }

    void PlayMusicGame::reflesh(const NotesData& notes)
    {
        //譜面取得
        m_playNotesData = PlayNotesData{ notes , PlayContext::IsPracticePlay()};

        m_totalNotes = m_playNotesData.getTotalNotes();

        const MusicData nowMusic = notes.getMusic();
        m_title = nowMusic.getArtistName() + U" - " + nowMusic.getMusicName();

        this->reflesh();
    }

    void PlayMusicGame::synchroCount(double& count)
    {
        m_playNotesData.synchroCount(m_sound, count);
    }

    void PlayMusicGame::update()
    {
        if (m_interruptProcess.resume()) {
            return;
        }
        if (!m_isStart) {
            m_playNotesData.update(m_sound, m_nowCount, m_score);
            m_interruptProcess.reset(std::bind(&PlayMusicGame::onReadyProcess, this));
            m_isStart = true;
            return;
        }
        // キー入力更新
        InputManager::Update();

        // ノーツ処理
        m_playNotesData.update(m_sound, m_nowCount, m_score);

        // フルコン演出
        if (m_score.m_maxCombo >= m_totalNotes && !m_FCAPAnime.isStart()) {
            m_FCAPAnime.play(m_score);
        }
        if (m_FCAPAnime.isStart())
            m_FCAPAnime.update();

        const uint64 sample = GetSamplePosU(m_sound);

        //曲の終わり
        if (!m_isFinish) {
            if (sample >= this->finishSample()) {
                m_isFinish = true;
                this->stopSound();
            }
        }
        // ライフゼロ
        if (this->isDead()) {
            m_interruptProcess.reset(std::bind(&PlayMusicGame::onDeadProcess, this));
        }
    }

    void PlayMusicGame::notesInit()
    {
        //譜面の初期化
        m_playNotesData.reset();
        m_score = Score(Game::Config().m_lifeGauge);
        m_isStart = false;
        m_isFinish = false;
        m_isDead = false;
        m_usePostProcess = false;
        m_interruptProcess.clear();
        m_flush.reset();
    }

    const PlayNotesData& PlayMusicGame::getPlayNotesData() const
    {
        return m_playNotesData;
    }

    void PlayMusicGame::setCourseMode(const Score& score)
    {
        m_isCourse = true;
        m_score.m_lifeHistory[0] = score.m_life;
        m_score.m_initLife = score.m_life;
        m_score.m_life = score.m_life;
        m_score.m_gauge = score.m_gauge;
    }

    void PlayMusicGame::ScoreUpdate(Score::Judge judge, s3d::int64 diff, NoteType type, NoteType baseType, bool playSe)
    {
        if (!g_pScore) {
            Print << U"Warning: Missing Score";
        }

        g_pScore->add(judge, diff);
        ::HandleAddJudgeEffect(judge, diff, type, baseType);

        if (playSe && judge != Score::Miss) {
            SoundManager::PlayInGameSe(JudgeStr(judge));
        }
    }
    void PlayMusicGame::drawBG(const double drawCount)const
    {
        // 背景描画
        if (Game::Config().m_useBgEffect) {
            Shaders::Blend()
                .setColor(ColorFx::GetColor())
                .apply([&] {
                    m_playBG->apply(drawCount);

                 });
        } else {
            m_playBG->apply(drawCount);
        }

        const double brightness = static_cast<double>(Game::Config().m_bgBrightness) / 10.0;
        if (brightness < 1) {
            Scene::Rect().draw(ColorF(0, 1 - brightness));
        }

        //スペクトラム描画
        if (Game::Config().m_isSpectrum) {
            m_spectrum.draw(m_sound);
        }
        TextureAsset(U"mainbg_back").draw(ColorF(1, brightness));
        TextureAsset(U"mainbg_front").draw();
    }

    void PlayMusicGame::draw(bool preview) const
    {
        {
            ScopedRenderTarget2D scopedRt(m_postProcessTex);
            m_postProcessTex.clear(s3d::ColorF(0, 1));

            const double drawCount = m_playNotesData.calDrawCount(m_nowCount);

            /**********/
            //背景
            this->drawBG(drawCount);

            {
                double f = beatRate();

                constexpr s3d::int32 w = 80;
                ColorF c1 = ColorF(0, 0, 0, 0.6 * (1 - f));
                ColorF c2 = m_playNotesData.getColor();
                c2.setA(0);

                Rect(0, 0, w, 600).draw({ c1,c2, c2,c1 });
                Rect(800, 0, -w, 600).draw({ c1,c2, c2,c1 });
            }
            //入力アクション
            const bool redInput = !m_isDead && InputManager::IsRedPressed();
            const bool blueInput = !m_isDead && InputManager::IsBluePressed();
            const bool yellowInput = !m_isDead && InputManager::IsYellowPressed();

            PlayStyle::Instance()->drawFrame(redInput, blueInput, yellowInput,
                [&] {
                    m_playNotesData.draw(drawCount, m_scrollRate);
                });
        }        
        if (m_usePostProcess) {
            auto scoped = Shaders::GrayGlitch().start();
            m_postProcessTex.draw();
        } else {
            m_postProcessTex.draw();
        }

        this->uiDraw(preview);

        m_flush.draw();
    }

    void PlayMusicGame::playModeDraw() const
    {
        this->drawAutoPlay(true);
    }

    void PlayMusicGame::previewDraw(const double count) const
    {
        const double drawCount = m_playNotesData.calDrawCount(count);

        //背景
        this->drawBG(drawCount);

        PlayStyle::Instance()->drawPreview([&] {
            m_playNotesData.previewDraw(drawCount, m_scrollRate);
        });

        this->drawMusicTitle(true);
        this->drawAutoPlay(true);
        this->drawNotesLevel();
    }

    void PlayMusicGame::drawCurrentBPM() const
    {
        BPMType bpm = m_playNotesData.getCurrentBPM();
        s3d::String tmp = U"{:.2f}*{:.1f}={:.2f}"_fmt(bpm, m_scrollRate, bpm * static_cast<double>(m_scrollRate));
        PutText(tmp, { 20, Scene::Height() - 100 });
    }

    void PlayMusicGame::drawAutoPlay(bool preview) const
    {
        const double headerOffset = preview ? 40 : 0;

        SharedDraw::DrawPlayContextHeader({ Scene::Center().x, 40 + headerOffset });
    }

    void PlayMusicGame::drawRandomMode() const
    {
        SharedDraw::DrawRandomNoteType(Vec2{ 800 - 10, Scene::Height() - 55 });
    }

    void PlayMusicGame::drawMusicTitle(bool preview) const
    {
        const double headerOffset = preview ? 40 : 0;
        PutText(m_title, { Scene::Center().x, 20 + headerOffset });
    }

    void PlayMusicGame::drawNotesLevel() const
    {
        const auto levelName = m_playNotesData.getLevelNameAndLevel();
        PutText(levelName, Vec2{ Scene::Center().x, Scene::Height() - 20 });
    }

    s3d::uint64 PlayMusicGame::finishSample() const
    {
        return Min(m_audioEndSample, m_playNotesData.getLastSample()) + 44100;
    }
    bool PlayMusicGame::isDead() const
    {
        if (!(m_isCourse || Game::Config().m_isLifeDead)) {
            return false;
        }
        // ライフ0以下
        return m_score.m_life <= 0;
    }
    double PlayMusicGame::beatRate() const
    {
        s3d::int32 beat = NotesData::RESOLUTION / 4;
        return Abs(static_cast<double>(static_cast<s3d::int32>(m_nowCount) % beat)) / static_cast<double>(beat);
    }
    Coro::Fiber<> PlayMusicGame::onReadyProcess()
    {
        if (m_isPreview) {
            m_barXEasing.jumpToB();
            co_return;
        }
        co_await Coro::FiberUtil::WaitForSeconds(1.0s);

        m_readyAnime.playIn();
        while (!m_readyAnime.isEnd()) {
            m_readyAnime.update(Scene::DeltaTime());
            co_yield{};
        }
        TimeLite::Timer timer{ 0.5 };
        while (!timer.isEnd() || KeyControl.pressed()) {
            timer.update(Scene::DeltaTime());
            co_yield{};
        }
        m_readyAnime.playOut();
        m_barXEasing.start();
        g_startTimer.restart();
        while (!m_readyAnime.isEnd() || g_startTimer.ms() <= 1500) {
            m_readyAnime.update(Scene::DeltaTime());
            co_yield{};
        }
        SoundManager::PlayInGameMusic(m_sound);
        co_return;
    }
    Coro::Fiber<> PlayMusicGame::onDeadProcess()
    {
        SoundManager::PlaySe(U"dead");
        m_sound.pause(0.5s);
        m_isDead = true;
        m_usePostProcess = true;

        // フラッシュ
        m_flush.start(0.5);
        while (m_flush.isActive()) {
            m_flush.update(Scene::DeltaTime());
            co_yield{};
        }
        co_await Coro::FiberUtil::WaitForSeconds(0.5s);
        m_isFinish = true;
        co_await Coro::FiberUtil::Stop();
    }
    bool PlayMusicGame::isFinish() const
    {
        return m_isFinish && (m_FCAPAnime.isEnd() || !m_FCAPAnime.isStart());
    }

    void PlayMusicGame::uiDraw(bool preview) const
    {
        //UI***************************************************************

        const IndicateRate rateType = Game::Config().m_rateType;
        IndicateRate subRateType = Game::Config().m_subRateType;
        const IndicateRate clearRateType =
            rateType != IndicateRate::Life    ? rateType :
            subRateType != IndicateRate::Life ? subRateType :
                                                IndicateRate::Up
            ;
        if (m_isCourse) {
            subRateType = clearRateType;
        }
        const float lifeRate = ResultRank::CalcLifeRate(m_score);
        const float upClearRate = ResultRank::CalcClearRate(m_score, m_totalNotes);
        const float downClearRate = ResultRank::CalcClearRateAsDownType(m_score, m_totalNotes);

        const auto rate =
            (m_isCourse || rateType == IndicateRate::Life) ? lifeRate :
                          rateType == IndicateRate::Down ? downClearRate :
                                                           upClearRate
            ;
        s3d::Optional<float> subRate = s3d::none;
        if (Game::Config().m_useSubRate) {
            subRate =
                subRateType == IndicateRate::Life ? lifeRate :
                subRateType == IndicateRate::Down ? downClearRate :
                                                    upClearRate
                ;
        }
        PlayStyle::Instance()->drawComboAndRate(m_score.m_currentCombo, rate, subRate);

        //曲の現在地
        const double invBarXRate = (1.0 - m_barXEasing.easeInOut());
        {
            const double barRate = Saturate(GetSamplePos(m_sound) / static_cast<double>(finishSample()));

            SamplePosView{}
                .setOffs(-invBarXRate * 50)
                .setRate(barRate)
                .draw();
        }
        // ゲージ
        {
            const float clearRate = clearRateType == IndicateRate::Down ? downClearRate : upClearRate;

            GaugeView{}
                .setOffs(invBarXRate * 60.0)
                .setClearRate(clearRate)
                .setLifeRate(lifeRate)
                .setGaugeKind(m_score.m_gauge)
                .setBeatRate(beatRate())
                .draw()
                ;
        }

        if (!m_isPreview) {
            //FCアニメ
            if (m_FCAPAnime.isStart()) {
                if (!m_FCAPAnime.isEnd()) {
                    m_FCAPAnime.draw();
                }
            }
            // Readyアニメ
            if (m_readyAnime.isPlaying()) {
                m_readyAnime.draw();
            }
            // スタートアニメ
            if (!preview && g_startTimer.ms() <= 1500) {
                StartAnime::Draw(g_startTimer.ms() / 1500.0);
            }
        }

        this->drawMusicTitle(preview);
        this->drawAutoPlay(preview);
        this->drawNotesLevel();
        this->drawRandomMode();
    }
}