#include"CoursePlay.h"
#include"Useful.hpp"

CoursePlay::CoursePlay() :
	m_font1(16, Typeface::Bold),
	m_font2(12)
{
	SoundAsset(L"title").stop(1s);
}

void CoursePlay::init()
{
	auto& musics = Game::Musics();
	auto& currentCourse = Game::Courses().at(m_data->m_selectCourse);
	auto& notesID = currentCourse.getNotesIDs()[m_data->m_currentCourseIndex];

	const String& id = musics[notesID.first].getSoundNameID();
	const Vec2& loop = musics[notesID.first].getLoopRange();
	Wave wav = SoundAsset(id).getWave();

	const size_t sample = 22050 + wav.lengthSample;

	//無音作成
	auto sam = WaveSample(0, 0);
	wav.reserve(sample);
	//wavに4秒間のオフセット追加
	wav.insert(wav.begin() + 44100 * loop.x, 22050, sam);

	m_sound = Sound(wav);
	SoundAsset::Release(id);

	if (!m_sound.isPlaying())
	{
		m_sound.setLoopBySec(true, loop.x + 0.5, loop.y + 0.5);
		m_sound.setPosSec(loop.x);
		m_sound.play(3s);
	}

}

void CoursePlay::update()
{
	auto& musics = Game::Musics();
	auto& currentCourse = Game::Courses().at(m_data->m_selectCourse);

	++m_timer;
	if (PlayKey::Start().clicked)
	{
		auto& notesID = currentCourse.getNotesIDs()[m_data->m_currentCourseIndex];
		SoundManager::SE::Play(L"desisionLarge");
		m_data->m_nowMusics = musics[notesID.first];
		m_data->m_selectMusic = musics[notesID.first].getIndex();
		m_data->m_selectLevel = notesID.second;
		changeScene(SceneName::Main, 2000, false);

	}

	m_highSpeed.update(m_data->m_scrollRate);
}

namespace
{
	void NonSelectMusicBannerBG(int x, int y, const Array<MusicData>& musics, int index, int offsetX, int offstY, const Font& font)
	{

		TextureAsset(L"bannerbg").resize(200, 270).drawAt(x + offsetX, y + 25 + offstY);
		musics[index].getTexture().resize(160, 160).drawAt(x + offsetX, y - 15 + offstY).drawFrame(0, 2);

		util::ContractionDrawbleString(font(musics[index].getMusicName()), { x + offsetX, y + 100 + offstY }, 195);
	}
}
void CoursePlay::highSpeedDraw()const
{
	auto& currentCourse = Game::Courses().at(m_data->m_selectCourse);
	auto& notesID = currentCourse.getNotesIDs()[m_data->m_currentCourseIndex];
	auto& music = Game::Musics().at(notesID.first);


	String tmp = Format(music.getBPM(), L"*", m_data->m_scrollRate);

	const auto kineticFunction = [=](KineticTypography& k)
	{
		static int fBpm = 0;
		static int eBpm = 0;

		if (k.ch == '*')
			fBpm = k.index;
		if (k.ch == '=')
			eBpm = k.index;
		if (Input::KeyControl.pressed)
			if (k.index > fBpm&&k.index < eBpm)
				k.col = Palette::Red;

	};

	TextureAsset(L"label").draw(0, 500);

	if (const auto result = EvaluateOpt(tmp))
	{
		m_font2(tmp, L"=", result.value()).drawKinetic(10, 530, kineticFunction);
	}
	if (Input::KeyControl.pressed)
	{
		m_highSpeed.draw(music.getMinSoundBeat(), music.getMaxSoundBeat(), m_data->m_scrollRate);
	}

}

void CoursePlay::musicInfoDraw() const
{
	auto& currentCourse = Game::Courses().at(m_data->m_selectCourse);
	auto& notesID = currentCourse.getNotesIDs()[m_data->m_currentCourseIndex];
	auto& music = Game::Musics().at(notesID.first);

	//左の曲
	if (m_data->m_currentCourseIndex > 0)
	{
		auto& preID = currentCourse.getNotesIDs()[m_data->m_currentCourseIndex - 1];
		NonSelectMusicBannerBG(100, 250, Game::Musics(), preID.first, 0, 0, m_font2);
	}
	//右の曲
	if (m_data->m_currentCourseIndex < currentCourse.getNotesIDs().size() - 1)
	{
		auto& nextID = currentCourse.getNotesIDs()[m_data->m_currentCourseIndex + 1];
		NonSelectMusicBannerBG(700, 250, Game::Musics(), nextID.first, 0, 0, m_font2);
	}
	//ジャケ絵部分
	TextureAsset(L"bannerbg").resize(300, 400).drawAt(400, 295);
	music.getTexture().resize(230, 230).drawAt(400, 250 - 15).drawFrame(0, 2);

	//曲情報

	//作曲家
	util::ContractionDrawbleString(m_font2(music.getArtistName()), { 400, 440 }, 280);

	//BPM
	m_font2(Format(L"BPM:", music.getBPM())).draw(440, 470, Palette::Black);

	//文字の描画範囲調整
	util::ContractionDrawbleString(m_font1(music.getMusicName()), { 400, 400 }, 280);

}

void CoursePlay::draw() const
{
	TextureAsset(L"coursebg").uv((m_timer % 800) / 800.0, 0, 1, 1).draw(0, 0);


	TextureAsset(L"label").draw();
	this->musicInfoDraw();
	this->highSpeedDraw();
	FontAsset(L"label")(L"Course").draw(10, 33);

	SceneInfo::Draw(L"Ctrl+↑↓:ハイスピード変更　Enter:開始");

}

//--------------------------------------------------------------------------------
//関数：drawFadeIn
//--------------------------------------------------------------------------------
void CoursePlay::drawFadeIn(double t) const
{
	FadeIn(Fade::FlipPage, t, [this]() {this->draw(); }, true);
}

//--------------------------------------------------------------------------------
//関数：drawFadeOut
//--------------------------------------------------------------------------------
void CoursePlay::drawFadeOut(double t) const
{
	if (m_data->m_toScene == SceneName::Main)
	{
		auto& currentCourse = Game::Courses().at(m_data->m_selectCourse);
		auto& notesID = currentCourse.getNotesIDs()[m_data->m_currentCourseIndex];
		auto& music = Game::Musics().at(notesID.first);

		this->draw();
		FadeOut(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
		const double size = EaseOut(300.0, 350.0, Easing::Cubic, t);
		music.getTexture().resize(size, size).drawAt(400, 300, ColorF(1, t*t));
	}
	else
	{
		this->draw();
	}
}