#include"ResultScene.h"
#include "Useful.hpp"
#include"AutoPlayManager.h"
#include"ResultRank.h"
#include"ResultEffect.h"
#include"FontKinetic.h"
ResultScene::ResultScene() :
	m_timer(0),
	m_font(8, L"Straight", FontStyle::Outline),
	m_artistFont(12),
	m_rateFont(7, L"Straight", FontStyle::Outline),
	m_nameFont(16, Typeface::Bold),
	m_clearRate(0.0f),
	m_notesSum(0),
	m_specialResult(SpecialResult::None),
	m_isNewRecord(false),
	m_isClear(false),
	m_startEffect(false),
	m_backLight(TextureAsset(L"backLight"), { 5,8 }, 1),
	m_passEffectEasing(3, 1, Easing::Quad, 800)
{
	SoundAsset(L"result").play();
	m_font.changeOutlineStyle(TextOutlineStyle(Palette::White, Palette::White, 1));
	m_font.changeOutlineStyle(TextOutlineStyle(Palette::White, Palette::White, 1));
}

ResultScene::~ResultScene()
{}
void ResultScene::init()
{
	ClearPrint();
	m_resultScore = m_data->m_resultScore;

	auto& judges = m_resultScore.m_judgeCount;

	//ノーツ数
	m_notesSum = m_data->m_nowMusics.getNotesData()[m_data->m_selectLevel].getTotalNotes();

	//ＡＰ処理
	if (judges[Score::Perfect] == m_notesSum)
	{
		m_specialResult = SpecialResult::All_Perfect;
	}
	//フルコン処理
	else if (m_resultScore.m_currentCombo == m_notesSum)
	{
		m_specialResult = SpecialResult::Full_Combo;
	}

	m_clearRate = ResultRank::calcClearRate(m_resultScore, m_notesSum);

	bool isClear = m_clearRate >= 80 ? true : false;

	m_isClear = isClear;

	/*EasingSet*/
	for (unsigned int i = 0; i < Score::TERM; ++i)
		m_judgeEasing[i] = EasingController<int>(0, judges[i], Easing::Linear, 1500);

	m_rateEasing = EasingController<double>(0, m_clearRate, Easing::Linear, 1500);
	m_totalNotesEasing = EasingController<int>(0, m_notesSum, Easing::Linear, 1500);
	m_maxComboEasing = EasingController<int>(0, m_resultScore.m_maxCombo, Easing::Linear, 1500);

	for (auto&& elm : m_judgeEasing)
		elm.start();

	m_rateEasing.start();
	m_maxComboEasing.start();
	m_totalNotesEasing.start();

	//autoPlayならデータ保存はしない
	if (AutoPlayManager::IsAutoPlay)
		return;
	bool newRecord = false;
	auto& currentMusic = Game::Musics()[m_data->m_selectMusic];
	auto& currentLevel = currentMusic.getNotesData()[m_data->m_selectLevel];
	if (isClear)
	{
		if (!currentLevel.isClear)
		{
			currentLevel.isClear = true;
			newRecord = true;
		}
	}
	if (m_specialResult > currentLevel.specialResult)
	{
		currentLevel.specialResult = m_specialResult;
		newRecord = true;

	}
	if (m_clearRate > currentLevel.clearRate)
	{
		currentLevel.clearRate = m_clearRate;
		newRecord = true;
		m_isNewRecord = true;
	}
	if (newRecord)
	{
		BinaryWriter writer(L"Score/" + currentMusic.getGenreName() + L"/" + currentMusic.getFileName() + L"/" + currentLevel.getFileName() + L".bin");

		writer.write(currentLevel.isClear);
		writer.write(static_cast<unsigned int>(currentLevel.specialResult));
		writer.write(currentLevel.clearRate);
	}

	if (m_data->m_isCoursePlay)
	{
		float life = m_data->m_life;
		if (life <= 0)
			m_passEffect = PassEffect::Failure;

		m_data->m_currentCourseIndex++;

		if (life > 0 && m_data->m_currentCourseIndex >= Game::Courses()[m_data->m_selectCourse].getNotesIDs().size())
		{
			m_passEffect = PassEffect::Pass;

			auto& course = Game::Courses()[m_data->m_selectCourse];
			course.save(true);
			course.setClear();
		}
	}

}
void ResultScene::update()
{
	if (m_passEffect != PassEffect::None)
	{
		if (m_rateEasing.isEnd() && !m_passEffectEasing.isEnd())
		{
			m_passEffectEasing.start();
		}
		if (m_passEffectEasing.isEnd())
		{
			if (PlayKey::Start().clicked && !m_isPassEffectEnd)
			{
				m_isPassEffectEnd = true;
				return;
			}
		}
	}
	m_backLight.update();

	if (!m_startEffect)
	{
		if (m_rateEasing.isEnd())
		{

			m_effect.add<ResultEffect>(ResultRank::getRankTextureName(m_clearRate), Vec2{ 640,170 }, 0.5);
			if (m_isNewRecord)
				m_effect.add<ResultEffect>(L"newRecord", Vec2{ 400, 300 });
			if (m_isClear)
				m_effect.add<ResultEffect>(L"resultClear", Vec2{ 200, 350 }, 0.2);
			else
				m_effect.add<ResultEffect>(L"resultFailed", Vec2{ 200, 350 }, 0.2);

			m_startEffect = true;
		}

	}

	if (m_tweet.update())
		return;

	if (Input::KeyT.clicked)
	{
		if (m_data->m_isCoursePlay)
		{
			auto& cource = Game::Courses()[m_data->m_selectCourse];
			String text = cource.getTitle() +
				(m_passEffect == PassEffect::Pass ? L"に合格" :
				 m_passEffect == PassEffect::None ? L"をプレイ中":
													L"に不合格") + L"\n#ColorfulTone";
			m_tweet.tweet(text);

		}
		else
		{
			auto& music = m_data->m_nowMusics;
			String text =
				music.getMusicName() + L"/"
				+ music.getNotesData()[m_data->m_selectLevel].getLevelName() + L"で"
				+ Format(m_clearRate) + L"%達成\n#ColorfulTone";
			m_tweet.tweet(text);
		}
	}

	if (PlayKey::Start().clicked || PlayKey::BigBack().clicked)
	{
		SoundAsset(L"result").stop();
		SoundAsset::Release(L"result");
		SoundManager::SE::Play(L"desisionLarge");

		if (m_data->m_isCoursePlay)
		{
			if (m_passEffect == PassEffect::Failure || m_data->m_currentCourseIndex >= Game::Courses()[m_data->m_selectCourse].getNotesIDs().size())
			{
				changeScene(SceneName::CourseSelect, 1000);
			}
			else
			{
				changeScene(SceneName::Course, 1000);
			}
		}
		else
		{
			changeScene(SceneName::Select, 1000);
		}
	}
}

void ResultScene::draw()const
{
	auto& music = m_data->m_nowMusics;
	auto& tex = music.getTexture();
	static PixelShader ps(L"Shaders/ResultBG.ps");
	TextureAsset(L"resultBG1").draw();



	Graphics2D::BeginPS(ps);
	{
		tex.resize(800, 800).drawAt(400, 300);
	}
	Graphics2D::EndPS();

	m_backLight.scale(4).drawAt({ 400, 300 });

	TextureAsset(L"resultBG2").draw();

	TextureAsset(L"label").draw();

	FontAsset(L"label")(L"RESULT").draw(10, 33);
	tex.resize(230, 230).drawAt(155, 250).drawFrame(0, 2);

	//文字の描画範囲調整
	{
		util::ContractionDrawbleString(m_nameFont(music.getMusicName()), { 155,400 }, 280);
	}

	{
		const auto note = music.getNotesData()[m_data->m_selectLevel];
		const auto name = note.getLevelName() + L" Lv" + Format(note.getLevel());
		util::ContractionDrawbleString(m_artistFont(name), { 155,430 }, 280, note.getColor());
	}
	{
		util::ContractionDrawbleString(m_artistFont(music.getArtistName()), { 155,460 }, 280);
	}
	m_effect.update();

	//レート
	m_rateFont(L"{:.2f}%"_fmt, m_rateEasing.easeInOut()).drawCenter(400, 300, Palette::Black);

	//スコア
	if (m_rateEasing.isEnd())
	{

		TextureAsset(ResultRank::getRankTextureName(m_clearRate)).scale(0.5).drawAt(640, 170);

		if (m_specialResult == SpecialResult::All_Perfect)
			TextureAsset(L"apLogo").scale(0.5).drawAt(640, 250);
		else if (m_specialResult == SpecialResult::Full_Combo)
			TextureAsset(L"fcLogo").scale(0.5).drawAt(640, 250);


		if (m_isNewRecord)
			TextureAsset(L"newRecord").drawAt(400, 300);

		const String clearResult = m_isClear ? L"resultClear" : L"resultFailed";

		TextureAsset(clearResult).scale(0.20).drawAt(202, 352, Palette::Black);
		TextureAsset(clearResult).scale(0.20).drawAt(200, 350);


	}
	const int baseX = 540;
	const int baseY = 280;

	m_font(L"PERFECT").draw(baseX, baseY);
	m_font(Pad(m_judgeEasing[Score::Perfect].easeInOut(), { 6,L' ' })).drawKinetic(baseX + 120, baseY, FontKinetic::DeleteSpace);
	m_font(L"GREAT").draw(baseX, baseY + 30);
	m_font(Pad(m_judgeEasing[Score::Great].easeInOut(), { 6,L' ' })).drawKinetic(baseX + 120, baseY + 30, FontKinetic::DeleteSpace);
	m_font(L"GOOD").draw(baseX, baseY + 60);
	m_font(Pad(m_judgeEasing[Score::Good].easeInOut(), { 6,L' ' })).drawKinetic(baseX + 120, baseY + 60, FontKinetic::DeleteSpace);
	m_font(L"MISS").draw(baseX, baseY + 90);
	m_font(Pad(m_judgeEasing[Score::Miss].easeInOut(), { 6,L' ' })).drawKinetic(baseX + 120, baseY + 90, FontKinetic::DeleteSpace);

	Line(baseX, baseY + 117, baseX + 210, baseY + 117).draw(2);

	m_font(L"TOTAL").draw(baseX, baseY + 130);
	m_font(Pad(m_totalNotesEasing.easeInOut(), { 6,L' ' })).drawKinetic(baseX + 120, baseY + 130, FontKinetic::DeleteSpace);
	m_font(L"MAX COMBO").drawKinetic(baseX, baseY + 160, FontKinetic::DeleteSpace);
	m_font(Pad(m_maxComboEasing.easeInOut(), { 6,L' ' })).drawKinetic(baseX + 120, baseY + 160, FontKinetic::DeleteSpace);

	SceneInfo::Draw(L"T:リザルトをツイート   Press Enter");

	//合格エフェクト
	if (m_passEffect != PassEffect::None)
	{
		if (!m_isPassEffectEnd && (m_passEffectEasing.isActive() || m_passEffectEasing.isEnd()))
		{
			Window::BaseClientRect().draw(ColorF(0, 0.8));

			if (m_passEffect == PassEffect::Pass)
			{
				TextureAsset(L"pass").scale(m_passEffectEasing.easeInOut()).drawAt(400, 300);
			}
			else if (m_passEffect == PassEffect::Failure)
			{
				TextureAsset(L"noPass").scale(m_passEffectEasing.easeInOut()).drawAt(400, 300);
			}
		}
	}

}

//--------------------------------------------------------------------------------
//関数：drawFadeIn
//--------------------------------------------------------------------------------
void ResultScene::drawFadeIn(double t) const
{
	draw();
	FadeIn(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
}

//--------------------------------------------------------------------------------
//関数：drawFadeOut
//--------------------------------------------------------------------------------
void ResultScene::drawFadeOut(double t) const
{
	draw();
}