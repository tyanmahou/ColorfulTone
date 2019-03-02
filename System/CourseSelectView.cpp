#include "CourseSelectView.hpp"
#include "CourseSelect.hpp"
#include "SharedDraw.hpp"
#include "CourseGenre.hpp"
#include "Useful.hpp"
namespace
{
	using Action = CourseSelect::Action;

	void DrawBG()
	{
		static auto& tex = TextureAsset(L"memo2")(75, 22, 400, 110);
		for (size_t index = 0; index < 4; ++index)
		{
			tex.draw(10, index *115 + 95).drawFrame(0, 1, Color(224, 209, 173));
		}
	}
	void DrawMusicInfo(int y, int musicID, int notesID)
	{
		static const FontAsset font16b(L"selectMusics");
		static const FontAsset font12(L"bpm");

		const auto& musics = Game::Musics();
		const auto& music = musics[musicID];
		const auto& notes = musics[musicID][notesID];

		const Vec2 jacketPos{ 70, y + 60 };
		constexpr Vec2 jacketSize{ 100.0,100.0 };
		RectF(jacketPos - jacketSize / 2.0, jacketSize).draw(notes.getColor());

		music
			.getTexture()
			.resize(jacketSize)
			.rotate(Math::Radians(-7.0))
			.drawAt(jacketPos);

		util::ContractionDrawbleString(
			font16b(musics[musicID].getMusicName()), 
			{ 130 ,y + 15 }, 
			270,
			Palette::Black, 
			false
		);
		const String name = notes.getLevelName() + L" Lv" + Format(notes.getLevel());
		util::ContractionDrawbleString(
			font12(name), 
			{ 130 ,y + 45 },
			270,
			Palette::Black, 
			false
		);
	}

	void DrawCourseMusics(const CourseData* pCourse)
	{
		if (!pCourse)
		{
			return;
		}
		int i = 0;
		for (auto& notes : pCourse->getNotesIDs())
		{
			::DrawMusicInfo(i * 115 + 90, notes.first, notes.second);
			++i;
		}
	}
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
		::DrawBG();

		auto select = CourseSelect::GetSelectInfo();

		const auto& genres = CourseGenreManager::Genres();
		const CourseGenre* pGenre = genres.size()
			? &genres[select.genre] : nullptr;

		const auto& courses = m_pScene->getCourses();
		const CourseData* pCourse = courses.size()
			? &courses[select.course] : nullptr;

		const auto action = m_pScene->getAction();

		// コース情報表示
		if (action == Action::CourseSelect)
		{
			::DrawCourseMusics(pCourse);
		}

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
					FontAsset(L"selectMusics")(L"★").drawCenter(pos + Vec2{ 30, 30 });
				}
			}).draw(
				courses,
				select.course,
				[](const CourseData& c)->decltype(auto) {return c.getTitle(); }
			);
		}

		// TODO memo


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
