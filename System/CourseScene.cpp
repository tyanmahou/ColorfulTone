#include"CourseScene.h"
#include"Fade.h"
#include"SceneInfo.h"
#include"AutoPlayManager.h"
#include"PlayKey.h"
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
}

void CourseSelectScene::update()
{
	auto& courses = Game::Instance()->m_courses;

	++m_timer;
	if (!courses.empty())
	{
		if (PlayKey::Up().clicked)
		{
			SoundAsset(L"select").playMulti(0.5);
			m_selectCourse--;
			if (m_selectCourse < 0)
				m_selectCourse += courses.size();
		}
		else if (PlayKey::Down().clicked)
		{
			SoundAsset(L"select").playMulti(0.5);
			m_selectCourse++;
		}
		m_selectCourse %= courses.size();

		if (PlayKey::Start().clicked)
		{
			if (courses.at(m_selectCourse).canPlay())
			{
				SoundAsset(L"desisionLarge").playMulti();
				m_data->m_currentCourseIndex = 0;
				m_data->m_isCoursePlay = true;
				m_data->m_selectCourse = m_selectCourse;
				m_data->m_life = 100.0f;
				//絶対Autoは解除する
				AutoPlayManager::Instance()->m_autoPlay = false;
				changeScene(L"course", 3000);
			}
			else
				MessageBox::Show(L"全ての譜面データが存在していないので、このコースはプレイできません。");
		}
	}
	//戻る
	if (PlayKey::BigBack().clicked)
	{
		SoundAsset(L"cancel").playMulti(0.5);
		changeScene(L"title", 3000);
		m_data->m_isCoursePlay = false;
	}


}

namespace
{

	//テキストをうまくリサイズする
	void ResizeText(const Font& font, const String& text, const Vec2& pos, const Color& color = Palette::White, double size = 280)
	{
		const auto nameLength = font(text).region().w;
		if (nameLength > size)
		{
			const auto scale = size / nameLength;
			Graphics2D::SetTransform(Mat3x2::Translate(-pos.x, -pos.y).scale(scale, 1).translate(pos.x, pos.y));
		}
		font(text).draw(pos.x, pos.y,color);
		Graphics2D::SetTransform(Mat3x2::Identity());

	}

}
//譜面情報の表示
void CourseSelectScene::musicInfo(int y, int musicID, int notesID) const

{
	auto& musics = Game::Instance()->m_musics;
	constexpr int x = 10;
	constexpr int size = 100;
	Rect(x, y, 800, size + 20).draw(ColorF(0, 0.8));
	musics[musicID].getTexture().resize(size, size).draw(x + 10, y + 10);

	ResizeText(m_font, musics[musicID].getMusicName(), { x + 120 ,y + 30 });
	{
		const auto& note = musics[musicID].getNotesData().at(notesID);
		const String name = note.getLevelName() + L" Lv" + Format(note.getLevel());
		ResizeText(m_levelFont, name, { x + 120 ,y + 60 },note.getColor());
	}
}

void CourseSelectScene::draw() const
{
	TextureAsset(L"coursebg").uv((m_timer % 800) / 800.0, 0, 1, 1).draw(0, 0);

	auto& courses = Game::Instance()->m_courses;
	if (courses.size())
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

			{
				const auto nameLength = m_font(courses[index].getTitle()).region().w;
				if (nameLength > 206.0)
				{
					const auto scale = 206.0 / nameLength;
					Graphics2D::SetTransform(Mat3x2::Translate(-(430 + 90 + offset), -(10 + 60 * i)).scale(scale, 1).translate(430 + 90 + offset, 10 + 60 * i));
				}
				m_font(courses[index].getTitle()).draw(430 + 90 + offset, 10 + 60 * i);
				Graphics2D::SetTransform(Mat3x2::Identity());
			}
			if(courses[index].isClear())
			m_font(L"★").draw(455 + offset, 10 + 60 * i);

			//TextureAsset(ResultRank::getRankTextureName(notesData[index].clearRate)).scale(0.1).drawAt(430 + 320 + offset, 10 + 15 + 60 * i);

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
	FadeIn(t, Fade::Default);

}

//--------------------------------------------------------------------------------
//関数：drawFadeOut
//--------------------------------------------------------------------------------
void CourseSelectScene::drawFadeOut(double t) const
{

	draw();
	FadeOut(t, Fade::Default);

}