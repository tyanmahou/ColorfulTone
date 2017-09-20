#include"MusicSelectScene.h"
#include"Fade.h"
#include"GenreManager.h"
#include"AutoPlayManager.h"
#include"ResultRank.h"
#include"SceneInfo.h"
#include"PlayKey.h"
namespace {

	static unsigned int g_nowPlayMusic = 0;
	static Sound g_sampleSound;
	void AudioPlay(const String id, const MusicData::ABLoop loop)
	{

		Wave wav = SoundAsset(id).getWave();

		const size_t sample = 22050 + wav.lengthSample;

		//�����쐬
		auto sam = WaveSample(0, 0);
		wav.reserve(sample);
		//wav��4�b�Ԃ̃I�t�Z�b�g�ǉ�
		wav.insert(wav.begin() + 44100 * loop.x, 22050, sam);

		g_sampleSound = Sound(wav);
		SoundAsset::Release(id);

		if (!g_sampleSound.isPlaying())
		{
			g_sampleSound.setLoopBySec(true, loop.x + 0.5, loop.y + 0.5);
			g_sampleSound.setPosSec(loop.x);
			g_sampleSound.play(3s);
		}

	}
	void AudioStop()
	{
		g_sampleSound.stop(1s);
	}
}
//--------------------------------------------------------------------------------
//�ÓI�����o�ϐ�
//--------------------------------------------------------------------------------

unsigned int MusicSelectScene::m_selectMusic;
unsigned int MusicSelectScene::m_selectLevel;
unsigned int MusicSelectScene::m_selectGenre;
bool MusicSelectScene::m_levelInfoMode;

std::future<void> MusicSelectScene::m_audioResult = std::async(std::launch::async, []() {return; });

//--------------------------------------------------------------------------------
//�֐��F�R���X�g���N�^
//--------------------------------------------------------------------------------
MusicSelectScene::MusicSelectScene() :
	m_timer(0),
	m_font1(16, Typeface::Bold),
	m_font2(12),
	m_font3(6, L"Straight", FontStyle::Bold),
	m_rateFont(12, L"Straight", FontStyle::Outline),
	m_mode(Mode::MusicSelect),
	m_changeMainScene(false),
	m_rightShift(0, 300, Easing::Linear, 200),
	m_leftShift(0, -300, Easing::Linear, 200),
	m_bannerSize(160, 230, Easing::Linear, 100),
	m_bannerBGSize({ 200,270 }, { 300, 400 }, Easing::Linear, 100),
	m_bannerBGY(250 + 25, 300 - 5, Easing::Linear, 100),
	m_infoSeat(0, 300, Easing::Expo, 200),
	m_isMusicDefineEasing(0, 600, Easing::Quart, 800),
	m_isGenreDefineEasing(0, 600, Easing::Quart, 800)
{
	ClearPrint();

	m_rateFont.changeOutlineStyle(TextOutlineStyle(Palette::White, Palette::White, 2));

	SoundAsset(L"title").stop();
	SoundAsset::Release(L"title");

	m_bannerSize.start();
	m_bannerBGSize.start();
	m_bannerBGY.start();
	m_infoSeat.start();

}

//--------------------------------------------------------------------------------
//�֐��F�f�X�g���N�^
//--------------------------------------------------------------------------------
MusicSelectScene::~MusicSelectScene()
{

}
void Quit()
{

	SoundAsset::ReleaseByTag(L"MusicData");
	g_sampleSound.release();
}
//--------------------------------------------------------------
//�֐��Finit
//--------------------------------------------------------------------------------

void MusicSelectScene::init()
{
	//�y�Ȑ�
	m_musics = Game::Instance()->m_musics;
	Erase_if(m_musics, GenreManager::m_refiners[m_selectGenre].m_refiner);
	m_musicsSize = m_musics.size();

	m_audioResult = std::async(std::launch::async, AudioPlay, m_musics[m_selectMusic].getSoundNameID(), m_musics[m_selectMusic].getLoopRange());
}
//--------------------------------------------------------------------------------
//�֐��FeasingStartBySelectMusic
//--------------------------------------------------------------------------------
//�T�v�F�A�j���[�V�����p
//--------------------------------------------------------------------------------

void MusicSelectScene::EasingStartBySelectMusic()
{
	m_bannerSize.reset();
	m_bannerBGSize.reset();
	m_bannerBGY.reset();
	m_infoSeat.reset();

}
//--------------------------------------------------------------------------------
//�֐��FeasingEndBySelectMusic
//--------------------------------------------------------------------------------
//�T�v�F�A�j���[�V�����p
//--------------------------------------------------------------------------------
void MusicSelectScene::EasingEndBySelectMusic()
{
	m_bannerSize.start();
	m_bannerBGSize.start();
	m_bannerBGY.start();
	m_infoSeat.start();

}

//--------------------------------------------------------------------------------
//�֐��Fupdate
//--------------------------------------------------------------------------------
void MusicSelectScene::update()
{
	m_timer++;
	const auto previousSelectIndex = m_selectMusic;

	if (m_rightShift.isEnd())
	{
		m_rightShift.reset();
		if (m_mode == Mode::MusicSelect)
		{
			m_selectMusic += m_musicsSize;
			--m_selectMusic;
		}
		else
		{
			m_selectGenre += GenreManager::m_refiners.size();
			--m_selectGenre;
		}
		EasingEndBySelectMusic();
	}
	if (m_leftShift.isEnd())
	{
		m_leftShift.reset();
		if (m_mode == Mode::MusicSelect)
			m_selectMusic++;
		else
			m_selectGenre++;

		EasingEndBySelectMusic();
	}

	m_selectMusic = m_musicsSize ? m_selectMusic%m_musicsSize : 0;

	const unsigned int genreSize = GenreManager::m_refiners.size();

	m_selectGenre = genreSize ? m_selectGenre%genreSize : 0;

	const unsigned int levelSelectMax = m_musicsSize ? m_musics[m_selectMusic].getNotesData().size() : 0;

	m_selectLevel = levelSelectMax? m_selectLevel%levelSelectMax:0;

	//�L�[�{�[�h����
	if (!m_isMusicDefineEasing.isActive() && !Input::KeyControl.pressed && !m_isGenreDefineEasing.isActive())
	{
		//�y�ȑI���̏���
		if (m_mode == Mode::MusicSelect || m_mode == Mode::GenreSelect)
		{
			if (!m_rightShift.isActive() && !m_leftShift.isActive())
			{
					//�Ȃ̑I��
					if (PlayKey::Left().pressed)
					{
						SoundAsset(L"select").playMulti(0.5);
						EasingStartBySelectMusic();
						m_rightShift.start();
					}
					else if (PlayKey::Right().pressed)
					{
						SoundAsset(L"select").playMulti(0.5);
						EasingStartBySelectMusic();
						m_leftShift.start();
					}
					//��,�W�������̌���
					if (PlayKey::Start().clicked)
					{
						if (m_mode == Mode::MusicSelect)
						{
							if (m_musicsSize)
							{

								SoundAsset(L"desisionSmall").playMulti(0.5);
								m_mode = Mode::LevelSelect;
								m_isMusicDefineEasing.start();
							}
						}
						else if (m_mode == Mode::GenreSelect)
						{
							SoundAsset(L"desisionSmall").playMulti(0.5);
							static unsigned int nowGenre = 0;
							if (m_selectGenre != nowGenre)
							{
								m_audioResult.wait();
								AudioStop();

								nowGenre = m_selectGenre;

								m_musics = Game::Instance()->m_musics;
								Erase_if(m_musics, GenreManager::m_refiners[m_selectGenre].m_refiner);
								m_musicsSize = m_musics.size();

								if (m_musicsSize)
								{
									m_selectMusic %= m_musicsSize;

									m_audioResult = std::async(std::launch::async, AudioPlay, m_musics[m_selectMusic].getSoundNameID(), m_musics[m_selectMusic].getLoopRange());
								}
							}
							m_mode = Mode::MusicSelect;
							m_isGenreDefineEasing.start();


						}
					}
				
				//�W�������I���ɖ߂�
				if (m_mode == Mode::MusicSelect&&PlayKey::SmallBack().clicked)
				{
					SoundAsset(L"cancel").playMulti(0.5);
					m_mode = Mode::GenreSelect;
					m_isGenreDefineEasing.start();
				}

			}
		}
		else	//���x���I����
		{
			//�ȑI���ɖ߂�
			if (PlayKey::SmallBack().clicked)
			{
				SoundAsset(L"cancel").playMulti(0.5);
				m_mode = Mode::MusicSelect;
				m_isMusicDefineEasing.start();
			}
			if (m_musics[m_selectMusic].getNotesData().size() > 0)
			{
				if (PlayKey::Up().clicked)
				{
					SoundAsset(L"select").playMulti(0.5);
					m_selectLevel += m_musics[m_selectMusic].getNotesData().size();
					m_selectLevel--;
				}
				else
					if (PlayKey::Down().clicked)
					{
						SoundAsset(L"select").playMulti(0.5);
						m_selectLevel++;
					}
				//���x������ƃV�[���J��
				if (PlayKey::Start().clicked)
				{
					SoundAsset(L"desisionLarge").playMulti();
					m_data->m_nowMusics = m_musics[m_selectMusic];
					m_data->m_selectMusic = m_musics[m_selectMusic].getIndex();
					m_data->m_selectLevel = m_selectLevel;
					m_changeMainScene = true;
					m_audioResult.wait();
					Quit();
					changeScene(L"main", 3000);
				}
			}
		}
	}

	//��������
	if (m_musicsSize)
	{

		if (previousSelectIndex != m_selectMusic)
		{
			if (!PlayKey::Right().pressed && !PlayKey::Left().pressed)
			{
				m_audioResult.wait();
				if (g_nowPlayMusic != m_selectMusic)
					AudioStop();

				m_audioResult = std::async(std::launch::async, AudioPlay, m_musics[m_selectMusic].getSoundNameID(), m_musics[m_selectMusic].getLoopRange());
				g_nowPlayMusic = m_selectMusic;
			}
		}
	}




	//�n�C�X�s�ύX
	m_highSpeedDemo.update(m_data->m_scrollRate);

	//�߂�
	if (PlayKey::BigBack().clicked)
	{
		SoundAsset(L"cancel").playMulti(0.5);
		m_audioResult.wait();
		Quit();
		changeScene(L"title", 3000);
	}

	//�v���C���[�h
	if (Input::KeyF1.clicked)
	{
		SoundAsset(L"desisionSmall").playMulti(0.5);
		AutoPlayManager::Instance()->m_autoPlay = !(AutoPlayManager::Instance()->m_autoPlay);
	}

	//���؂�ւ�
	if (Input::KeyShift.clicked)
	{
		SoundAsset(L"desisionSmall").playMulti(0.5);
		m_levelInfoMode = !m_levelInfoMode;
	}
}

namespace
{
	void NonSelectMusicBannerBG(int x, int y, const Array<MusicData>& musics, int index, int offsetX, int offstY, const Font& font)
	{

		TextureAsset(L"bannerbg").resize(200, 270).drawAt(x + offsetX, y + 25 + offstY);
		musics[index].getTexture().resize(160, 160).drawAt(x + offsetX, y - 15 + offstY).drawFrame(0, 2);

		const auto nameLength = font(musics[index].getMusicName()).region().w;
		if (nameLength > 195)
		{
			const auto scale = 195.0 / nameLength;
			Graphics2D::SetTransform(Mat3x2::Translate(Vec2{ (x + offsetX)*(1.0 / scale - 1.0),0 })*Mat3x2::Scale(Vec2{ scale, 1.0 }));
		}

		font(musics[index].getMusicName()).drawCenter(x + offsetX, y + 100 + offstY);
		Graphics2D::SetTransform(Mat3x2::Identity());

	}
	void NonSelectGenreBannerBG(int x, int y, int index, int offsetX, int offstY, const Font& font)
	{
		auto &genres = GenreManager::m_refiners;

		TextureAsset(L"bannerbg").resize(200, 270).drawAt(x + offsetX, y + 25 + offstY);

		genres[index].getTexture().resize(160, 160).drawAt(x + offsetX, y - 15 + offstY).drawFrame(0, 2);


		const auto nameLength = font(genres[index].m_name).region().w;
		if (nameLength > 195)
		{
			const auto scale = 195.0 / nameLength;
			Graphics2D::SetTransform(Mat3x2::Translate(Vec2{ (x + offsetX)*(1.0 / scale - 1.0),0 })*Mat3x2::Scale(Vec2{ scale, 1.0 }));
		}

		font(genres[index].m_name).drawCenter(x + offsetX, y + 100 + offstY);
		Graphics2D::SetTransform(Mat3x2::Identity());

	}
}
void MusicSelectScene::genreBannerAndInfoDraw()const
{
	const auto& genres = GenreManager::m_refiners;
	const auto genresSize = genres.size();
	const auto easingOffsetX = m_rightShift.easeInOut() + m_leftShift.easeInOut();
	const int y = 250;
	const auto genreOffsetY = m_isGenreDefineEasing.easeInOut() - 600;
	//���̋�
	NonSelectGenreBannerBG(-200, y, (genresSize + m_selectGenre - 2) % genresSize, easingOffsetX, genreOffsetY, m_font2);
	NonSelectGenreBannerBG(100, y, (genresSize + m_selectGenre - 1) % genresSize, easingOffsetX, genreOffsetY, m_font2);
	//�E�̋�
	NonSelectGenreBannerBG(700, y, (m_selectGenre + 1) % genresSize, easingOffsetX, genreOffsetY, m_font2);
	NonSelectGenreBannerBG(1000, y, (m_selectGenre + 2) % genresSize, easingOffsetX, genreOffsetY, m_font2);

	//--------------------------------------------------------------------------------------------------------------
	//�I�𒆂̋�
	//--------------------------------------------------------------------------------------------------------------

	//�W���P�G����
	TextureAsset(L"bannerbg").resize(m_bannerBGSize.easeInOut()).drawAt(400 + easingOffsetX, m_bannerBGY.easeInOut() + genreOffsetY);
	genres[m_selectGenre].getTexture().resize(m_bannerSize.easeInOut(), m_bannerSize.easeInOut()).drawAt(400 + easingOffsetX, y - 15 + genreOffsetY).drawFrame(0, 2);

	//�ȏ��
	Graphics2D::SetStencilState(StencilState::Replace);
	Graphics2D::SetStencilValue(2);
	Rect(400 - m_infoSeat.easeInOut() / 2, 350 - m_infoSeat.easeInOut() / 2, m_infoSeat.easeInOut(), m_infoSeat.easeInOut()).draw(ColorF(1, 0.5));

	Graphics2D::SetStencilState(StencilState::Test(StencilFunc::Equal));

	//�����̕`��͈͒���
	const auto nameLength = m_font1(genres[m_selectGenre].m_name).region().w;
	if (nameLength > 280)
	{
		const auto scale = 280.0 / nameLength;
		Graphics2D::SetTransform(Mat3x2::Translate(Vec2{ 400.0*(1.0 / scale - 1.0),genreOffsetY })*Mat3x2::Scale(Vec2{ scale, 1.0 }));
	}
	//�Ȗ�
	m_font1(genres[m_selectGenre].m_name).drawCenter(400, 400 + genreOffsetY);
	Graphics2D::SetStencilState(StencilState::Default);

	//�ړ����̏���
	if (!(m_bannerSize.isActive() || m_bannerSize.isEnd()))
	{
		const auto nameLength = m_font2(genres[m_selectGenre].m_name).region().w;
		if (nameLength > 210)
		{
			const auto scale = 210.0 / nameLength;
			Graphics2D::SetTransform(Mat3x2::Translate(Vec2{ (400 + easingOffsetX)*(1.0 / scale - 1.0),genreOffsetY })*Mat3x2::Scale(Vec2{ scale, 1.0 }));
		}
		m_font2(genres[m_selectGenre].m_name).drawCenter(400 + easingOffsetX, y + 100 + genreOffsetY);
	}
	Graphics2D::SetTransform(Mat3x2::Identity());
	//--------------------------------------------------------------------------------------------------------------


}

void MusicSelectScene::musicBannerAndInfoDraw()const
{
	auto &musics = m_musics;
	const auto easingOffsetX = m_rightShift.easeInOut() + m_leftShift.easeInOut();
	const int y = 250;


	const auto easingOffestY = m_isMusicDefineEasing.easeInOut();
	const auto genreOffsetY = m_isGenreDefineEasing.easeInOut();
	//���̋�
	NonSelectMusicBannerBG(-200, y, musics, (m_musicsSize + m_selectMusic - 2) % m_musicsSize, easingOffsetX, genreOffsetY - easingOffestY, m_font2);
	NonSelectMusicBannerBG(100, y, musics, (m_musicsSize + m_selectMusic - 1) % m_musicsSize, easingOffsetX, genreOffsetY - easingOffestY, m_font2);
	//�E�̋�
	NonSelectMusicBannerBG(700, y, musics, (m_selectMusic + 1) % m_musicsSize, easingOffsetX, genreOffsetY + easingOffestY, m_font2);
	NonSelectMusicBannerBG(1000, y, musics, (m_selectMusic + 2) % m_musicsSize, easingOffsetX, genreOffsetY + easingOffestY, m_font2);

	//--------------------------------------------------------------------------------------------------------------
	//�I�𒆂̋�
	//--------------------------------------------------------------------------------------------------------------
	Graphics2D::SetTransform(Mat3x2::Translate(Vec2{ -easingOffestY / 3,genreOffsetY }));

	//�W���P�G����
	TextureAsset(L"bannerbg").resize(m_bannerBGSize.easeInOut()).drawAt(400 + easingOffsetX, m_bannerBGY.easeInOut());
	musics[m_selectMusic].getTexture().resize(m_bannerSize.easeInOut(), m_bannerSize.easeInOut()).drawAt(400 + easingOffsetX, y - 15).drawFrame(0, 2);

	//�ȏ��
	Graphics2D::SetStencilState(StencilState::Replace);
	Graphics2D::SetStencilValue(2);
	Rect(400 - m_infoSeat.easeInOut() / 2, 350 - m_infoSeat.easeInOut() / 2, m_infoSeat.easeInOut(), m_infoSeat.easeInOut()).draw(ColorF(1, 0.5));

	Graphics2D::SetStencilState(StencilState::Test(StencilFunc::Equal));

	//��ȉ�
	{
		auto&str = musics[m_selectMusic].getArtistName();
		const auto nameLength = m_font2(str).region().w;
		if (nameLength > 280)
		{
			const auto scale = 280.0 / nameLength;
			Graphics2D::SetTransform(Mat3x2::Translate(Vec2{ 400.0*(1.0 / scale - 1.0) - easingOffestY / 3.0 / scale,genreOffsetY })*Mat3x2::Scale(Vec2{ scale, 1.0 }));
		}
		m_font2(str).drawCenter(400, 440, Palette::White);
		Graphics2D::SetTransform(Mat3x2::Translate(Vec2{ -easingOffestY / 3,genreOffsetY }));
	}

	//BPM
	m_font2(Format(L"BPM:", musics[m_selectMusic].getBPM())).draw(440, 470, Palette::Black);

	//�����̕`��͈͒���
	{
		const auto nameLength = m_font1(musics[m_selectMusic].getMusicName()).region().w;
		if (nameLength > 280)
		{
			const auto scale = 280.0 / nameLength;
			Graphics2D::SetTransform(Mat3x2::Translate(Vec2{ 400.0*(1.0 / scale - 1.0) - easingOffestY / 3.0 / scale,genreOffsetY })*Mat3x2::Scale(Vec2{ scale, 1.0 }));
		}
		//�Ȗ�
		m_font1(musics[m_selectMusic].getMusicName()).drawCenter(400, 400);
	}
	Graphics2D::SetStencilState(StencilState::Default);
	//�ړ����̏���
	if (!(m_bannerSize.isActive() || m_bannerSize.isEnd()))
	{
		const auto nameLength = m_font2(musics[m_selectMusic].getMusicName()).region().w;
		if (nameLength > 210)
		{
			const auto scale = 210.0 / nameLength;
			Graphics2D::SetTransform(Mat3x2::Translate(Vec2{ (400 + easingOffsetX)*(1.0 / scale - 1.0),genreOffsetY })*Mat3x2::Scale(Vec2{ scale, 1.0 }));
		}
		m_font2(musics[m_selectMusic].getMusicName()).drawCenter(400 + easingOffsetX, y + 100);
	}
	Graphics2D::SetTransform(Mat3x2::Identity());
	//--------------------------------------------------------------------------------------------------------------


}

void MusicSelectScene::highSpeedDraw()const
{
	String tmp = Format(m_musics[m_selectMusic].getBPM(), L"*", m_data->m_scrollRate);

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

	if (const auto result = EvaluateOpt(tmp))
	{
		m_font2(tmp, L"=", result.value()).drawKinetic(10, 530, kineticFunction);
	}
	if (Input::KeyControl.pressed)
	{
		auto& music = m_musics[m_selectMusic];
		m_highSpeedDemo.draw(music.getMinSoundBeat(), music.getMaxSoundBeat(), m_data->m_scrollRate);
	}

}

void MusicSelectScene::levelSelectDraw()const
{
	auto& notesData = m_musics[m_selectMusic].getNotesData();

	const int w = 30;
	for (unsigned i = 0, size = notesData.size(); i < size; ++i)
	{
		const Rect rect(800 - w - w * size + w * i, 520, w, w);
		rect.draw(HSV(i*360.0 / size, 0.5, 1)).drawFrame(0, 1, HSV(i*360.0 / size, 1, 1));
		if (m_levelInfoMode)
			TextureAsset(ResultRank::getRankTextureName(notesData[i].clearRate)).scale(0.05).drawAt(rect.center, Palette::Black);
		else
			FontAsset(L"info")(notesData[i].getLevel()).drawCenter(rect.center, Palette::Black);
	}

	const auto  transformMat = Mat3x2::Translate({ 400 - m_isMusicDefineEasing.easeInOut() * 400 / 600,0 });
	Graphics2D::SetTransform(transformMat);


	for (int i = 0; i < 10; ++i)
	{
		const auto offset = i == 4 ? !m_isMusicDefineEasing.isActive() && (PlayKey::Up().clicked || PlayKey::Down().clicked) && !Input::KeyControl.pressed ? -25 : -30 : 0;
		Color uiColor = i == 4 ? Palette::Yellow : Palette::White;
		TextureAsset(L"levelbg").draw(430 + offset, 60 * i, uiColor);
		const auto index = (m_selectLevel + i - 4 + notesData.size()*Abs(i - 4)) % notesData.size();

		{
			const auto nameLength = m_font1(notesData[index].getLevelName()).region().w;
			if (nameLength > 206.0)
			{
				const auto scale = 206.0 / nameLength;
				Graphics2D::SetTransform(Mat3x2::Translate(-(430 + 90 + offset), -(10 + 60 * i)).scale(scale, 1).translate(430 + 90 + offset, 10 + 60 * i)*transformMat);
			}
			m_font1(notesData[index].getLevelName()).draw(430 + 90 + offset, 10 + 60 * i, notesData[index].getColor());
			Graphics2D::SetTransform(transformMat);
		}

		m_rateFont(notesData[index].getLevel()).drawCenter(430 + 40 + offset, 10 + 15 + 60 * i);
		TextureAsset(ResultRank::getRankTextureName(notesData[index].clearRate)).scale(0.1).drawAt(430 + 320 + offset, 10 + 15 + 60 * i);

	}
	TextureAsset(L"levelMask").draw(430, 0, Palette::Black);

	//INFO
	Rect(400 + 50, 300 + 50, 300, 200).draw(ColorF(0, 0.8)).drawFrame(1, 0, Palette::White);

	if (m_selectLevel >= notesData.size())
		return;

	const auto clearRate = notesData[m_selectLevel].clearRate;
	TextureAsset(ResultRank::getRankTextureName(clearRate)).scale(0.25).drawAt(675, 400);


	m_font3(L"Rate").draw(460, 365);
	m_rateFont(L"{:.2f}%"_fmt, clearRate).drawCenter(600 - 60, 400);

	m_font3(L"NotesArtist").draw(460, 430);
	//
	{
		const auto nameLength = m_font2(notesData[m_selectLevel].getNotesArtistName()).region().w;
		if (nameLength > 196)
		{
			const auto scale = 196.0 / nameLength;
			Graphics2D::SetTransform(Mat3x2::Translate(-548, -462).scale(scale, 1).translate(548, 462)*transformMat);
		}
		m_font2(notesData[m_selectLevel].getNotesArtistName()).drawCenter(548, 462);
		Graphics2D::SetTransform(transformMat);
	}
	m_font3(L"TotalNotes").draw(460, 480);
	m_font2(notesData[m_selectLevel].getTotalNotes()).drawCenter(548, 514);
	if (notesData[m_selectLevel].isClear)
		TextureAsset(L"iconClear").scale(0.5).drawAt(600 + 75, 452);
	if (notesData[m_selectLevel].specialResult == SpecialResult::All_Perfect)
		TextureAsset(L"iconAP").scale(0.5).drawAt(600 + 75, 504);
	else if (notesData[m_selectLevel].specialResult == SpecialResult::Full_Combo)
		TextureAsset(L"iconFC").scale(0.5).drawAt(600 + 75, 504);

	Graphics2D::SetTransform(Mat3x2::Identity());
}
//--------------------------------------------------------------------------------
//�֐��Fdraw
//--------------------------------------------------------------------------------
void MusicSelectScene::draw()const
{
	//TextureAsset(L"selectbg").draw(-((m_timer/2)%800),0);
	//TextureAsset(L"selectbg").draw(-((m_timer/2)% 800)+800, 0);

	TextureAsset(L"selectbg").uv((m_timer % 800) / 800.0, 0, 1, 1).draw(0, 0);

	TextureAsset(L"label").draw(0, 500);

	if (m_musicsSize <= m_selectMusic&&m_musicsSize)
		return;

	if (m_musicsSize)
	{
		//���x���`��**********************************************************************************************

		levelSelectDraw();
		//**********************************************************************************************


		//TextureAsset(L"levelbg").resize(400,60).draw(400, 300);

		//�y�ȏ��
		musicBannerAndInfoDraw();
	}
	//�W���������
	genreBannerAndInfoDraw();


	{
		TextureAsset(L"modeBack").drawAt(100, 50, Palette::Black);
		auto& genres = GenreManager::m_refiners;
		const auto nameLength = m_font1(genres[m_selectGenre].m_name).region().w;
		if (nameLength > 180)
		{
			const auto scale = 180.0 / nameLength;
			Graphics2D::SetTransform(Mat3x2::Translate(-100, -50).scale(scale, 1).translate(100, 50));
		}
		m_font1(genres[m_selectGenre].m_name).drawCenter(100, 50);
		Graphics2D::SetTransform(Mat3x2::Identity());
	}


	//�n�C�X�s
	highSpeedDraw();

	//�I�[�g�v���C
	if (AutoPlayManager::Instance()->m_autoPlay)
		m_font1(L"AutoPlay").draw(0, 0, Palette::Black);

	if (m_timer % 400 <= 200)
		SceneInfo::Draw(L"Enter:����@BackSpace:�i�荞��,�߂�@Esc:�^�C�g���߂� ");
	else
		SceneInfo::Draw(L"Shift:�\�����[�h�ؑց@F1:�I�[�g�@Ctrl+����:�n�C�X�s�[�h�ύX");

}


//--------------------------------------------------------------------------------
//�֐��FdrawFadeIn
//--------------------------------------------------------------------------------
void MusicSelectScene::drawFadeIn(double t) const
{

	draw();
	FadeIn(t, Fade::Default);

}

//--------------------------------------------------------------------------------
//�֐��FdrawFadeOut
//--------------------------------------------------------------------------------
void MusicSelectScene::drawFadeOut(double t) const
{

	draw();
	FadeOut(t, Fade::Default);
	if (m_changeMainScene)
		m_musics[m_selectMusic].getTexture().resize(300, 300).drawAt(400, 300, ColorF(1, t*t));

}