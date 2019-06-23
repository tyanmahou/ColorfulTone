#include"CourseScene.hpp"
#include"Useful.hpp"

#include "HighSpeedDemo.h"
#include"Audition.hpp"
namespace
{
	using MemoInfo = CourseScene::MemoInfo;

	MemoInfo g_selectInfo = MemoInfo::Course;
}
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
		if (Input::KeyShift.clicked)
		{
			SoundManager::SE::Play(L"desisionSmall");
			g_selectInfo = g_selectInfo == MemoInfo::Course
				? MemoInfo::Notes : MemoInfo::Course;
		}
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
CourseScene::CourseScene() :
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
	if ((PlayKey::BigBack().clicked || PlayKey::SmallBack().clicked) && m_data->m_course.isStart() ||
		PlayKey::BigBack().pressedDuration >= 1000)
	{
		this->changeScene(SceneName::CourseSelect, 1000);
		SoundManager::SE::Play(L"desisionLarge");
	}
	if (PlayKey::Start().clicked)
	{
		m_data->m_nowNotes = m_pModel->nowNotes();
		this->changeScene(SceneName::Main, 2000, false);
		SoundManager::SE::Play(L"desisionLarge2");
	}
	m_view.update();
}

void CourseScene::finally()
{
	m_pModel->finally();
}

namespace
{
	String GetSceneInfo(bool isStart)
	{
		if (isStart)
		{
			return L"Ctrl+↑↓:ハイスピード変更　Enter:開始 Esc,BackSpace:コース選択に戻る";
		}
		return L"Ctrl+↑↓:ハイスピード変更　Enter:開始 Esc長押し:コース選択に戻る";
	}
}
void CourseScene::draw() const
{
	m_view.draw();
	bool isStart = m_data->m_course.isStart();
	if (!isStart && PlayKey::BigBack().pressed)
	{
		FontAsset(L"20")(L"Esc長押しで戻る").drawCenter(400, 400, ColorF(0, 0.3 + PlayKey::BigBack().pressedDuration*0.70 / 1000));
	}
	SceneInfo::Draw(::GetSceneInfo(isStart));
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

MemoInfo CourseScene::GetMemoInfo()
{
	return g_selectInfo;
}
