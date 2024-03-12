#include <scenes/Scene/Course/CourseScene.hpp>
#include <Useful.hpp>

#include <core/Play/HighSpeed/HighSpeedDemo.hpp>
#include <scenes/utils/Audition.hpp>
#include <scenes/Scene/Config/ConfigMain.hpp>
#include <Siv3D.hpp>

namespace
{
	using namespace ct;
	using MemoInfo = CourseScene::MemoInfo;

	MemoInfo g_selectInfo = MemoInfo::Course;
}
namespace ct
{
	class CourseScene::Model
	{
	private:
		GameData* m_data;
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
		void setData(GameData& data)
		{
			m_data = &data;
		}
		const NotesData& nowNotes()const
		{
			return m_data->m_course.getCurrentNotes();
		}
		void init()
		{
			m_audition.request(nowNotes().getMusic());
		}
		void update()
		{
			if (m_config.isActive()) {
				if (!m_config.update() || KeyF11.down()) {
					m_config.setActive(false);
					m_config.reset();
					SoundManager::PlaySe(U"cancel");
				}
				return;
			} else {
				if (KeyF11.down()) {
					m_config.setActive(true);
					SoundManager::PlaySe(U"desisionSmall");
				}
			}

			if (KeyShift.down()) {
				SoundManager::PlaySe(U"desisionSmall");
				g_selectInfo = g_selectInfo == MemoInfo::Course
					? MemoInfo::Notes : MemoInfo::Course;
			}
			m_highSpeed.update(m_data->m_scrollRate);

			// 1曲目でバックまたはバック長押し
			if ((PlayKey::BigBack().down() || PlayKey::SmallBack().down()) && m_data->m_course.isStart() ||
				PlayKey::BigBack().pressedDuration() >= 1000ms
				) {
				SoundManager::PlaySe(U"desisionLarge");
				if (m_onBack) {
					m_onBack();
				}
				return;
			}
			if (PlayKey::Start().down()) {
				m_data->m_nowNotes = this->nowNotes();
				SoundManager::PlaySe(U"desisionLarge2");
				if (m_onStart) {
					m_onStart();
				}
				return;
			}
		}
		void postUpdate()
		{
			m_audition.update();
		}
		void finally()
		{
			m_audition.clear();
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
	CourseScene::CourseScene(const InitData& init) :
		ISceneBase(init),
		m_pModel(std::make_shared<Model>()),
		m_view(this)
	{
		m_pModel->setData(getData());
		m_pModel->init();
		m_pModel->subscribeOnStart([this] {
			this->changeScene(SceneName::Main, 2000, CrossFade::No);
			});
		m_pModel->subscribeOnBack([this] {
			this->changeScene(SceneName::CourseSelect, 1000);
			});
	}
	void CourseScene::update()
	{
		m_pModel->update();
		m_pModel->postUpdate();

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
			if (isStart) {
				return U"F11:コンフィグ　Ctrl+↑↓:ハイスピード変更　Enter:開始 Esc,BackSpace:コース選択に戻る";
			}
			return U"F11:コンフィグ　Ctrl+↑↓:ハイスピード変更　Enter:開始 Esc長押し:コース選択に戻る";
		}
	}
	void CourseScene::draw() const
	{
		m_view.draw();

		const bool isStart = getData().m_course.isStart();
		if (!isStart) {
			SharedDraw::LongPressBack(PlayKey::BigBack(), U"Esc長押しで戻る");
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
		if (getData().m_toScene == SceneName::Main) {
			const MusicData music = getData().m_course.getCurrentNotes().getMusic();
			this->draw();
			FadeOut(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
			const double size = EaseOut(Easing::Cubic, 300.0, 350.0, t);
			music.getTexture().resized(size, size).drawAt(400, 300, ColorF(1, t * t));
		} else {
			this->draw();
		}
	}

	const PlayCourse& CourseScene::getPlay() const
	{
		return getData().m_course;
	}

	const HighSpeedDemo& CourseScene::getHighSpeedDemo() const
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
}
