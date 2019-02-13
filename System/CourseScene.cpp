#include"Useful.hpp"

#include"CourseScene.h"
#include"Fade.h"
#include"SceneInfo.h"
#include"AutoPlayManager.h"

#include"CourseGenre.hpp"

 int CourseSelectScene::m_selectGenre = 0;
 int CourseSelectScene::m_selectCourse = 0;

CourseSelectScene::CourseSelectScene() :
	m_levelFont(12),
	m_font(16, Typeface::Bold)
{
	if (!SoundAsset(L"title").isPlaying())
		SoundAsset(L"title").play();

}

void CourseSelectScene::init()
{
	m_selectCourse = m_data->m_selectCourse;
	m_courses = Game::Courses();
}

void CourseSelectScene::update()
{
	auto& courses = m_courses; 
	auto& genres = CourseGenreManager::Genres();
	++m_timer;
	if (!courses.empty() && !genres.empty())
	{
		if (PlayKey::Up().clicked)
		{
			SoundManager::SE::Play(L"select");
			if (m_mode == Mode::Course)
			{
				m_selectCourse--;
				if (m_selectCourse < 0)
					m_selectCourse += courses.size();
			}
			else
			{
				m_selectGenre--;
				if (m_selectGenre < 0)
					m_selectGenre += genres.size();
			}
		}
		else if (PlayKey::Down().clicked)
		{
			SoundManager::SE::Play(L"select");
			if (m_mode == Mode::Course)
				m_selectCourse++;
			else
				m_selectGenre++;
		}
		m_selectCourse %= courses.size();
		m_selectGenre %= genres.size();

		if (PlayKey::Start().clicked)
		{
			if (m_mode == Mode::Course)
			{
				if (courses.at(m_selectCourse).canPlay())
				{
					SoundManager::SE::Play(L"desisionLarge");
					m_data->m_currentCourseIndex = 0;
					m_data->m_isCoursePlay = true;
					m_data->m_selectCourse = m_courses[m_selectCourse].getIndex();
					m_data->m_life = 100.0f;
					//絶対Autoは解除する
					AutoPlayManager::SetAutoPlay(false);
					changeScene(L"course", 3000);
				}
				else
					MessageBox::Show(L"全ての譜面データが存在していないので、このコースはプレイできません。");
			}
			else
			{
				SoundManager::SE::Play(L"desisionSmall");
				m_mode = Mode::Course;
				m_courses = Game::Courses();
				Erase_if(m_courses,genres[m_selectGenre].getRefiner());
				m_selectCourse %= m_courses.size();
			}
		}
	}
	//戻る小
	if (PlayKey::SmallBack().clicked && m_mode == Mode::Course)
	{
		SoundManager::SE::Play(L"cancel");
		m_mode = Mode::Genre;
	}

	//戻る
	if (PlayKey::BigBack().clicked)
	{
		SoundManager::SE::Play(L"cancel");

		changeScene(L"title", 3000);
		m_data->m_isCoursePlay = false;
	}
}

namespace
{

}
//譜面情報の表示
void CourseSelectScene::musicInfo(int y, int musicID, int notesID) const
{
	auto& musics = Game::Musics();
	constexpr int x = 10;
	constexpr int size = 100;
	Rect(x, y, 800, size + 20).draw(ColorF(0, 0.8));
	musics[musicID].getTexture().resize(size, size).draw(x + 10, y + 10);

	util::ContractionDrawbleString(m_font(musics[musicID].getMusicName()), { x + 120 ,y + 30 }, 280, Palette::White, false);
	{
		const auto& note = musics[musicID].getNotesData().at(notesID);
		const String name = note.getLevelName() + L" Lv" + Format(note.getLevel());
		util::ContractionDrawbleString(m_levelFont(name), { x + 120 ,y + 60 }, 280, note.getColor(), false);
	}
}

void CourseSelectScene::draw() const
{
	TextureAsset(L"coursebg").uv((m_timer % 800) / 800.0, 0, 1, 1).draw(0, 0);

	auto& courses = this->m_courses;
	auto& genres = CourseGenreManager::Genres();
	if (m_mode == Mode::Course&&courses.size())
	{
		int i = 0;
		for (auto& notes : courses.at(m_selectCourse).getNotesIDs())
		{
			this->musicInfo(i * 115 + 90, notes.first, notes.second);
			++i;
		}

		for (int i = 0; i < 10; ++i)
		{
			const auto offset = i == 4 ? (PlayKey::Up().clicked || PlayKey::Down().clicked) ? -25 : -30 : 0;
			Color uiColor = i == 4 ? Palette::Yellow : Palette::White;
			TextureAsset(L"levelbg").draw(430 + offset, 60 * i, uiColor);
			const auto index = (m_selectCourse + i - 4 + courses.size()*Abs(i - 4)) % courses.size();

			util::ContractionDrawbleString(m_font(courses[index].getTitle()), { 430 + 90 + offset, 10 + 60 * i }, 206, Palette::White, false);

			if (courses[index].isClear())
				m_font(L"★").draw(455 + offset, 10 + 60 * i);
		}

	}
	else if (m_mode == Mode::Genre&&genres.size())
	{
		for (int i = 0; i < 10; ++i)
		{
			const auto offset = i == 4 ? (PlayKey::Up().clicked || PlayKey::Down().clicked) ? -25 : -30 : 0;
			Color uiColor = i == 4 ? Palette::Yellow : Palette::White;
			TextureAsset(L"levelbg").draw(430 + offset, 60 * i, uiColor);
			const auto index = (m_selectGenre + i - 4 + genres.size()*Abs(i - 4)) % genres.size();

			util::ContractionDrawbleString(m_font(genres[index].getName()), { 430 + 90 + offset, 10 + 60 * i }, 206, Palette::White, false);
		}
	}

	TextureAsset(L"levelMask").draw(430, 0, Palette::Black);

	TextureAsset(L"label").draw();

	FontAsset(L"label")(L"Course").draw(10, 33);

	SceneInfo::Draw(L"Enter:決定 Esc:タイトル戻る ");

}
//--------------------------------------------------------------------------------
//関数：drawFadeIn
//--------------------------------------------------------------------------------
void CourseSelectScene::drawFadeIn(double t) const
{

	draw();
	FadeIn(Fade::Default, t);

}

//--------------------------------------------------------------------------------
//関数：drawFadeOut
//--------------------------------------------------------------------------------
void CourseSelectScene::drawFadeOut(double t) const
{

	draw();
	FadeOut(Fade::Default, t);

}