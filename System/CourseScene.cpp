#include"CourseScene.hpp"
#include"Useful.hpp"

#include "HighSpeedDemo.h"
#include"Audition.hpp"

class CourseScene::Model
{
private:
	std::shared_ptr<GameData> m_data;
	Audition m_audition;
	HighSpeedDemo m_highSpeed;
public:
	void setData(std::shared_ptr<GameData> data)
	{
		m_data = data;
	}
	const NotesData& nowNotes()const
	{
		return m_data->m_course.getCurrentNotes();
	}
	void init()
	{
		m_audition.autoPlayAndStop(*nowNotes().getMusic());
	}
	void update()
	{
		m_highSpeed.update(m_data->m_scrollRate);
	}
	void finally()
	{
		m_audition.stop();
	}

	const HighSpeedDemo& getHighSpeedDemo()const
	{
		return m_highSpeed;
	}
};
CourseScene::CourseScene():
	m_pModel(std::make_shared<Model>()),
	m_view(this)
{}

void CourseScene::init()
{
	m_pModel->setData(m_data);
	m_pModel->init();
}

void CourseScene::update()
{
	m_pModel->update();

	// 1曲目でバックまたはバック長押し
	if (PlayKey::BigBack().clicked && m_data->m_course.isStart()|| 
		PlayKey::BigBack().pressedDuration >= 1000)
	{
		this->changeScene(SceneName::CourseSelect, 1000);
		SoundManager::SE::Play(L"desisionLarge");
	}
	if (PlayKey::Start().clicked)
	{
		m_data->m_nowNotes = m_pModel->nowNotes();
		this->changeScene(SceneName::Main, 2000, false);
		SoundManager::SE::Play(L"desisionLarge");
	}
	m_view.update();
}

void CourseScene::finally()
{
	m_pModel->finally();
}

void CourseScene::draw() const
{
	m_view.draw();
	SceneInfo::Draw(L"Ctrl+↑↓:ハイスピード変更　Enter:開始 Esc長押し:コース選択に戻る");
}

//--------------------------------------------------------------------------------
//関数：drawFadeIn
//--------------------------------------------------------------------------------
void CourseScene::drawFadeIn(double t) const
{
	FadeIn(Fade::FlipPage, t, [this]() {this->draw(); }, true);
}

//--------------------------------------------------------------------------------
//関数：drawFadeOut
//--------------------------------------------------------------------------------
void CourseScene::drawFadeOut(double t) const
{
	if (m_data->m_toScene == SceneName::Main)
	{
		const auto& pMusic = m_data->m_course.getCurrentNotes().getMusic();
		this->draw();
		FadeOut(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
		const double size = EaseOut(300.0, 350.0, Easing::Cubic, t);
		pMusic->getTexture().resize(size, size).drawAt(400, 300, ColorF(1, t*t));
	}
	else
	{
		this->draw();
	}
}

const PlayCourse & CourseScene::getPlay() const
{
	return m_data->m_course;
}

const HighSpeedDemo & CourseScene::getHighSpeedDemo() const
{
	return m_pModel->getHighSpeedDemo();
}
