#include <scenes/Scene/CourseSelect/CourseSelectScene.hpp>
#include <Useful.hpp>
#include <scenes/Scene/Config/ConfigMain.hpp>

#include <Siv3D.hpp>

namespace
{
	using namespace ct;
	using Action = CourseSelectScene::Action;

	CourseSelectScene::SelectCourseInfo g_selectInfo;

	// コースの絞りこみ
	void RefineCourses(Array<CourseData>& musics)
	{
		if (CourseGenreManager::Genres().size())
		{
			musics.remove_if([](const CourseData& genere) {
				return !CourseGenreManager::GetRefiner(g_selectInfo.genre)(genere);
			});
		}
	}
	// コースの絞り込み
	void InitCourses(Array<CourseData>& courses)
	{
		courses = Game::Courses();

		::RefineCourses(courses);

		size_t size = courses.size();
		if (size) {
			g_selectInfo.course %= size;
		}
		else {
			g_selectInfo.course = 0;
		}
	}
	uint32& GetSelectTarget(Action action)
	{
		switch (action)
		{
		case Action::GenreSelect: return g_selectInfo.genre;
		case Action::CourseSelect: return g_selectInfo.course;
		default:
			break;
		}
		return g_selectInfo.course;
	}
	size_t GetTargetSize(Action action, const Array<CourseData>& courses)
	{
		switch (action)
		{
		case Action::GenreSelect: return CourseGenreManager::Genres().size();
		case Action::CourseSelect: return courses.size();
		default:
			break;
		}
		return 0;
	}
}

namespace ct
{
	class CourseSelectScene::Model
	{
	public:
		Model()
		{
			m_config.setActive(false);
		}
		void setData(GameData& data)
		{
			m_data = &data;
		}
		void init()
		{
			::InitCourses(m_courses);
			if (m_data->m_fromScene == SceneName::Course ||
				m_data->m_fromScene == SceneName::Main) {
				m_action = Action::CourseSelect;
			}
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

			if (!m_stopwatch.isStarted()) {
				m_stopwatch.start();
			}
			m_prevAction = m_action;
			// 選択するターゲットの参照
			auto& target = ::GetSelectTarget(m_action);
			size_t size = ::GetTargetSize(m_action, m_courses);
			m_moveSelect = SharedLogic::MoveSelectV();
			if (m_moveSelect != 0) {
				if (m_moveSelect < 0) {
					++target;
				} else {
					target += static_cast<uint32>(size);
					--target;
				}
				SoundManager::PlaySe(U"select");
				m_stopwatch.reset();
			}
			target = size ? target % size : 0;

			// 決定ボタン
			if (PlayKey::Start().down() && size) {
				if (m_action == Action::GenreSelect) {
					::InitCourses(m_courses);

					m_action = Action::CourseSelect;
					SoundManager::PlaySe(U"desisionSmall");
					m_stopwatch.reset();
				} else if (m_action == Action::CourseSelect) {
					if (m_courses[g_selectInfo.course].canPlay()) {
						m_isSelectedCourse = true;
					} else {						
						System::MessageBoxOK(U"全ての譜面データが存在していないので、このコースはプレイできません。");
					}
				}
			}
			// キャンセルボタン
			if (PlayKey::SmallBack().down()) {
				if (m_action == Action::CourseSelect) {
					m_action = Action::GenreSelect;
					SoundManager::PlaySe(U"cancel");
				}
			}
			// 再度indexの調整
			{
				auto& target2 = ::GetSelectTarget(m_action);
				size_t size2 = ::GetTargetSize(m_action, m_courses);
				target2 = size2 ? target2 % size2 : 0;
			}
		}
		bool onChangeAction()
		{
			return m_action != m_prevAction;
		}
		const CourseData& getSelectCourse()const
		{
			return m_courses[g_selectInfo.course];
		}
		const Array<CourseData>& getCourses()const
		{
			return m_courses;
		}
		Action getAction()const
		{
			return m_action;
		}
		// previous , current
		std::pair<Action, Action> getChangeAction()const
		{
			return { m_prevAction ,m_action };
		}

		int32 getMoveSelect()const
		{
			return m_moveSelect;
		}
		bool isSelectedCourse()
		{
			return m_isSelectedCourse;
		}
		size_t entryPage() const
		{
			const auto& courses = getCourses();
			const CourseData* pCourse = courses.size()
				? &getSelectCourse() : nullptr;
			if (!pCourse) {
				return 0;
			}
			size_t entrySize = pCourse->getEntrySize();
			size_t pageSize = entrySize == 0 ? 1 : (entrySize - 1) / 4 + 1;
			return (m_stopwatch.s64() / 2) % pageSize;
		}

		const ConfigMain& getConfig()const
		{
			return m_config;
		}
	private:
		GameData* m_data;
		Action m_action = Action::GenreSelect;
		Action m_prevAction = Action::GenreSelect;
		s3d::int32 m_moveSelect = 0;
		Array<CourseData> m_courses;

		bool m_isSelectedCourse = false;

		Stopwatch m_stopwatch;

		ConfigMain m_config;
	};

	CourseSelectScene::CourseSelectScene(const InitData& init) :
		ISceneBase(init),
		m_pModel(std::make_shared<Model>()),
		m_view(this)
	{

		m_pModel->setData(getData());
		m_pModel->init();

		if (!AudioAsset(U"title").isPlaying()) {
			SoundManager::PlayBgm(U"title", 1s);
		}
		if (getData().m_fromScene == SceneName::Course ||
			getData().m_fromScene == SceneName::Main) {
			m_view.onChangeAction();
		}
	}

	void CourseSelectScene::update()
	{
		m_pModel->update();
		if (m_pModel->isSelectedCourse()) {
			this->changeScene(SceneName::Course, 1000);
			SoundManager::PlaySe(U"desisionLarge");
		} else if (PlayKey::BigBack().down()) {
			this->changeScene(U"title", 1000);
			SoundManager::PlaySe(U"desisionLarge");
		}
		m_view.update();
		if (m_pModel->onChangeAction()) {
			m_view.onChangeAction();
		}
	}

	void CourseSelectScene::finally()
	{
		if (getData().m_toScene == SceneName::Course) {
			SoundManager::StopBgm(U"title", 1s);
			// データ運搬
			getData().m_course.init(m_pModel->getSelectCourse());
			// ライフ引継ぎがあるためスコアを初期化しておく
			getData().m_resultScore = Score();

			// 絶対Autoは解除する
			PlayContext::Revert();
		} else {
			getData().m_course.exit();
		}
	}

	void CourseSelectScene::draw() const
	{
		m_view.draw();
		// シーン情報
		SceneInfo::DrawEsc(s3d::Palette::Black);
		SceneInfo::Header(U"\U000F0493 F11");
		SceneInfo::Draw(U"Enter:決定　BackSpace:戻る");
	}

	void CourseSelectScene::drawFadeIn(double t) const
	{
		if (getData().m_fromScene == SceneName::Course) {
			FadeOut(Fade::FlipPage, t, [this]() {draw(); }, false);
		} else {
			FadeIn(Fade::FlipPage, t, [this]() {draw(); }, true);
		}
	}

	void CourseSelectScene::drawFadeOut([[maybe_unused]]double t) const
	{
		this->draw();
	}

	CourseSelectScene::SelectCourseInfo CourseSelectScene::GetSelectInfo()
	{
		return g_selectInfo;
	}

	const Array<CourseData>& CourseSelectScene::getCourses() const
	{
		return m_pModel->getCourses();
	}

	Action CourseSelectScene::getAction() const
	{
		return m_pModel->getAction();
	}

	std::pair<Action, Action> CourseSelectScene::getChangeAction() const
	{
		return m_pModel->getChangeAction();
	}

	int32 CourseSelectScene::getMoveSelect() const
	{
		return m_pModel->getMoveSelect();
	}
	size_t CourseSelectScene::entryPage() const
	{
		return m_pModel->entryPage();
	}
	const ConfigMain& CourseSelectScene::getConfig() const
	{
		return m_pModel->getConfig();
	}
}
