#include "CourseSelectView.hpp"
#include "CourseSelect.hpp"
#include "SharedDraw.hpp"
#include "CourseGenre.hpp"
namespace
{
	using Action = CourseSelect::Action;

	void DrawTitle(const CourseGenre*const genre)
	{
		static String title = L"Course";
		const String* pGenreName = genre ? &genre->getName() : nullptr;
		SharedDraw::Sticky(&title, pGenreName);
	}
}
class CourseSelectView::Impl
{
	const CourseSelect*const m_pScene;
	EasingController<double> m_memoOffset;
	SharedDraw::DrawBGLight m_lights;
public:
	Impl(const CourseSelect*const scene) :
		m_pScene(scene),
		m_memoOffset(300, 0, Easing::Back, 1000)
	{}
	void onChangeAction()
	{
		const auto change = m_pScene->getChangeAction();
		const auto previous = change.first;
		const auto current = change.second;
		if (previous == Action::GenreSelect && current == Action::CourseSelect)
		{
			m_memoOffset.reset();
			m_memoOffset.start();
		}
		else if (previous == Action::CourseSelect && current == Action::GenreSelect)
		{
			m_memoOffset.reset();
			m_memoOffset.restart();
		}
	}

	void update()
	{
		m_lights.update();
	}

	void draw() const
	{
		TextureAsset(L"canvasBg").draw();
		m_lights.draw();
		auto select = CourseSelect::GetSelectInfo();

		const auto& genres = CourseGenreManager::Genres();
		const CourseGenre* pGenre = genres.size()
			? &genres[select.genre] : nullptr;

		const auto& courses = m_pScene->getCourses();

		const CourseData* pCourse = courses.size()
			? &courses[select.course] : nullptr;

		const auto action = m_pScene->getAction();
		const int moveSelect = m_pScene->getMoveSelect();

		static EasingController<double> easingAnime(0.0, -30.0, Easing::Linear, 100);
		if (moveSelect)
		{
			easingAnime.reset();
			easingAnime.start();
		}
		const double offset = easingAnime.isActive() ?
			easingAnime.easeInOut() :
			easingAnime.getEnd();

		// リスト表示
		if (action == Action::GenreSelect)
		{
			SharedDraw::Select<CourseGenre>()
				.setOffset(offset)
				.draw(
					genres,
					select.genre,
					[](const CourseGenre& g)->decltype(auto) {return g.getName(); }
			);
		}
		else if (action == Action::CourseSelect)
		{
			SharedDraw::Select<CourseData>()
				.setOffset(offset)
				.setDrawble([](const CourseData& c, Vec2 pos) {
				if (c.isClear())
				{
					// TODO カラーを変えたい
					FontAsset(L"selectMusics")(L"★").drawCenter(pos + Vec2{ 40, 25 });
				}
			}).draw(
				courses,
				select.course,
				[](const CourseData& c)->decltype(auto) {return c.getTitle(); }
			);
		}
		// コース情報表示
		// TODO

		// ジャンル名表示
		::DrawTitle(pGenre);
	}
};
CourseSelectView::CourseSelectView(const CourseSelect * const scene) :
	m_pImpl(std::make_shared<Impl>(scene))
{}

void CourseSelectView::update() const
{
	m_pImpl->update();
}

void CourseSelectView::onChangeAction() const
{
	m_pImpl->onChangeAction();
}

void CourseSelectView::draw() const
{
	m_pImpl->draw();
}
