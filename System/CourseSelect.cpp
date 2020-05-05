#include "CourseSelect.hpp"
#include"CourseGenre.hpp"

#include"Useful.hpp"
#include"Fade.h"
#include"AutoPlayManager.h"
#include"SharedLogic.hpp"

namespace
{
	using Action = CourseSelect::Action;

	CourseSelect::SelectCourseInfo g_selectInfo;

	// �R�[�X�̍i�肱��
	void RefineCourses(Array<CourseData>& musics)
	{
		if (CourseGenreManager::Genres().size())
		{
			util::Erase_not_if(musics, CourseGenreManager::GetRefiner(g_selectInfo.genre));
		}
	}
	// �R�[�X�̍i�荞��
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

class CourseSelect::Model
{
private:
	std::shared_ptr<GameData> m_data;
	Action m_action = Action::GenreSelect;
	Action m_prevAction = Action::GenreSelect;
	int m_moveSelect = 0;
	Array<CourseData> m_courses;

	bool m_isSelectedCourse;
public:
	void setData(std::shared_ptr<GameData> data)
	{
		m_data = data;
	}
	void init()
	{
		::InitCourses(m_courses);
		if (m_data->m_fromScene == SceneName::Course || 
			m_data->m_fromScene == SceneName::Main)
		{
			m_action = Action::CourseSelect;
		}
	}
	void update()
	{
		m_prevAction = m_action;
		// �I������^�[�Q�b�g�̎Q��
		auto &target = ::GetSelectTarget(m_action);
		size_t size = ::GetTargetSize(m_action, m_courses);
		m_moveSelect = SharedLogic::MoveSelect();
		if (m_moveSelect)
		{
			if (m_moveSelect < 0)
			{
				++target;
			}
			else
			{
				target += size;
				--target;
			}
			SoundManager::SE::Play(L"select");
		}
		target = size ? target % size : 0;

		// ����{�^��
		if (PlayKey::Start().clicked && size)
		{
			if (m_action == Action::GenreSelect)
			{
				::InitCourses(m_courses);

				m_action = Action::CourseSelect;
				SoundManager::SE::Play(L"desisionSmall");
			}
			else if (m_action == Action::CourseSelect)
			{
				if (m_courses[g_selectInfo.course].canPlay())
				{
					m_isSelectedCourse = true;
				} 
				else {
					MessageBox::Show(L"�S�Ă̕��ʃf�[�^�����݂��Ă��Ȃ��̂ŁA���̃R�[�X�̓v���C�ł��܂���B");
				}
			}
		}
		// �L�����Z���{�^��
		if (PlayKey::SmallBack().clicked)
		{
			if (m_action == Action::CourseSelect)
			{
				m_action = Action::GenreSelect;
				SoundManager::SE::Play(L"cancel");
			}
		}
		// �ēxindex�̒���
		{
			auto &target = ::GetSelectTarget(m_action);
			size_t size = ::GetTargetSize(m_action, m_courses);
			target = size ? target % size : 0;
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

	int getMoveSelect()const
	{
		return m_moveSelect;
	}
	bool isSelectedCourse()
	{
		return m_isSelectedCourse;
	}
};

CourseSelect::CourseSelect() :
	m_pModel(std::make_shared<Model>()),
	m_view(this)
{}

void CourseSelect::init()
{
	m_pModel->setData(m_data);
	m_pModel->init();

	if (!SoundAsset(L"title").isPlaying())
	{
		SoundManager::BGM::Play(L"title", 1s);
	}
	if (m_data->m_fromScene == SceneName::Course ||
		m_data->m_fromScene == SceneName::Main)
	{
		m_view.onChangeAction();
	}
}

void CourseSelect::update()
{
	m_pModel->update();
	if (m_pModel->isSelectedCourse())
	{
		this->changeScene(SceneName::Course, 1000);
		SoundManager::SE::Play(L"desisionLarge");
	}else if (PlayKey::BigBack().clicked)
	{
		this->changeScene(L"title", 1000);
		SoundManager::SE::Play(L"desisionLarge");
	}
	m_view.update();
	if (m_pModel->onChangeAction())
	{
		m_view.onChangeAction();
	}
}

void CourseSelect::finally()
{
	if (m_data->m_toScene == SceneName::Course)
	{
		SoundAsset(L"title").stop(1s);
		// �f�[�^�^��
		m_data->m_course.init(m_pModel->getSelectCourse());
		// ���C�t���p�������邽�߃X�R�A�����������Ă���
		m_data->m_resultScore = Score();

		//���Auto�͉�������
		AutoPlayManager::SetAutoPlay(false);
	}
	else {
		m_data->m_course.exit();
	}
}

void CourseSelect::draw() const
{
	m_view.draw();
	// �V�[�����
	SceneInfo::Draw(L"Enter:���� Esc:�^�C�g���߂�");
}

void CourseSelect::drawFadeIn(double t) const
{
	if (m_data->m_fromScene == SceneName::Course)
	{
		FadeOut(Fade::FlipPage, t, [this]() {draw(); }, false);
	}
	else
	{
		FadeIn(Fade::FlipPage, t, [this]() {draw(); }, true);
	}
}

void CourseSelect::drawFadeOut(double t) const
{
	this->draw();
}

CourseSelect::SelectCourseInfo CourseSelect::GetSelectInfo()
{
	return g_selectInfo;
}

const Array<CourseData>& CourseSelect::getCourses() const
{
	return m_pModel->getCourses();
}

Action CourseSelect::getAction() const
{
	return m_pModel->getAction();
}

std::pair<Action, Action> CourseSelect::getChangeAction() const
{
	return m_pModel->getChangeAction();
}

int CourseSelect::getMoveSelect() const
{
	return m_pModel->getMoveSelect();
}
