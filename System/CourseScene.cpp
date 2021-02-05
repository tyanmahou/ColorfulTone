#include"CourseScene.hpp"
#include"Useful.hpp"

#include "HighSpeedDemo.h"
#include "ConfigMain.hpp"
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
	ConfigMain m_config;

	std::function<void()> m_onStart;
	std::function<void()> m_onBack;
public:
	Model()
	{
		m_config.setActive(false);
	}
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
		if (m_config.isActive()) {
			if (!m_config.update() || Input::KeyF11.clicked) {
				m_config.setActive(false);
				m_config.reset();
				SoundManager::SE::Play(L"cancel");
			}
			return;
		} else {
			if (Input::KeyF11.clicked) {
				m_config.setActive(true);
				SoundManager::SE::Play(L"desisionSmall");
			}
		}

		if (Input::KeyShift.clicked)
		{
			SoundManager::SE::Play(L"desisionSmall");
			g_selectInfo = g_selectInfo == MemoInfo::Course
				? MemoInfo::Notes : MemoInfo::Course;
		}
		m_highSpeed.update(m_data->m_scrollRate);

		// 1曲目でバックまたはバック長押し
		if ((PlayKey::BigBack().clicked || PlayKey::SmallBack().clicked ) && m_data->m_course.isStart() ||
			PlayKey::BigBack().pressedDuration >= 1000
			) {
			SoundManager::SE::Play(L"desisionLarge");
			if (m_onBack) {
				m_onBack();
			}
			return;
		}
		if (PlayKey::Start().clicked) {
			m_data->m_nowNotes = this->nowNotes();
			SoundManager::SE::Play(L"desisionLarge2");
			if (m_onStart) {
				m_onStart();
			}
			return;
		}
	}
	void finally()
	{
		m_audition.stop();
	}

	const HighSpeedDemo& getHighSpeedDemo()const
	{
		return m_highSpeed;
	}
	const ConfigMain& getConfig()const
	{
		return m_config;
	}

	void subscribeOnStart(std::function<void()>&& callback)
	{
		m_onStart = std::move(callback);
	}
	void subscribeOnBack(std::function<void()>&& callback)
	{
		m_onBack = std::move(callback);
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
	m_pModel->subscribeOnStart([this] {
		this->changeScene(SceneName::Main, 2000, false);
	});
	m_pModel->subscribeOnBack([this] {
		this->changeScene(SceneName::CourseSelect, 1000);
	});
}

void CourseScene::update()
{
	m_pModel->update();
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
			return L"F11:コンフィグ　Ctrl+↑↓:ハイスピード変更　Enter:開始 Esc,BackSpace:コース選択に戻る";
		}
		return L"F11:コンフィグ　Ctrl+↑↓:ハイスピード変更　Enter:開始 Esc長押し:コース選択に戻る";
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

const ConfigMain& CourseScene::getConfig() const
{
	return m_pModel->getConfig();
}

MemoInfo CourseScene::GetMemoInfo()
{
	return g_selectInfo;
}
