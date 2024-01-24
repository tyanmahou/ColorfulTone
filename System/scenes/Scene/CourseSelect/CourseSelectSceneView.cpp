#include <scenes/Scene/CourseSelect/CourseSelectSceneView.hpp>
#include <scenes/Scene/CourseSelect/CourseSelectScene.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace
{
	using namespace ct;
	using Action = CourseSelectScene::Action;

	void DrawBG()
	{
		const TextureRegion tex = TextureAsset(U"memo2")(75, 22, 400, 110);
		for (size_t index = 0; index < 4; ++index)
		{
			tex
				.draw(10, static_cast<double>(index * 115 + 95))
				.drawFrame(0, 1, Color(224, 209, 173));
		}
	}
	void DrawMusicInfo(int y, int musicID, int notesID)
	{
		const FontAsset font16b(FontName::SelectMusic);
		const FontAsset font12(FontName::Bpm);

		const auto& musics = Game::Musics();
		const auto& music = musics[musicID];
		const auto& notes = musics[musicID][notesID];

		const Vec2 jacketPos{ 70, y + 60 };
		constexpr Vec2 jacketSize{ 100.0,100.0 };
		RectF(jacketPos - jacketSize / 2.0, jacketSize).draw(notes.getColor());

		music
			.getTexture()
			.resized(jacketSize)
			.rotated(Math::ToRadians(-7.0))
			.drawAt(jacketPos);

		ContractionDrawbleString(
			font16b(musics[musicID].getMusicName()), 
			{ 130 ,y + 15 }, 
			270,
			Palette::Black, 
			false
		);
		ContractionDrawbleString(
			font12(notes.getLevelNameAndLevel()), 
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
		static String title = U"COURSE";
		const String* pGenreName = genre ? &genre->getName() : nullptr;
		SharedDraw::Sticky(&title, pGenreName);
	}
}

namespace ct
{
	class CourseSelectSceneView::Impl
	{
		const CourseSelectScene* const m_pScene;
		EasingAB<double> m_memoOffset;
		SharedDraw::DrawBGLight m_lights;
	public:
		Impl(const CourseSelectScene* const scene) :
			m_pScene(scene),
			m_memoOffset(300, 0, Easing::Back, 1000)
		{}
		void onChangeAction()
		{
			const auto change = m_pScene->getChangeAction();
			const auto previous = change.first;
			const auto current = change.second;
			if (previous == Action::GenreSelect && current == Action::CourseSelect) {
				m_memoOffset.jumpToA();
				m_memoOffset.start();
			} else if (previous == Action::CourseSelect && current == Action::GenreSelect) {
				m_memoOffset.jumpToB();
				m_memoOffset.start();
			}
		}

		void update()
		{
			m_lights.update();
		}

		void draw() const
		{
			TextureAsset(U"canvasBg").draw();
			m_lights.draw();
			::DrawBG();

			auto select = CourseSelectScene::GetSelectInfo();

			const auto& genres = CourseGenreManager::Genres();
			const CourseGenre* pGenre = genres.size()
				? &genres[select.genre] : nullptr;

			const auto& courses = m_pScene->getCourses();
			const CourseData* pCourse = courses.size()
				? &courses[select.course] : nullptr;

			const auto action = m_pScene->getAction();

			// コース情報表示
			if (action == Action::CourseSelect) {
				::DrawCourseMusics(pCourse);
			}

			const int moveSelect = m_pScene->getMoveSelect();
			static EasingAB<double> easingAnime(0.0, -30.0, Easing::Linear, 100);
			if (moveSelect) {
				easingAnime.reset();
				easingAnime.start();
			}
			const double offset = easingAnime.isMoving() ?
				easingAnime.easeInOut() :
				easingAnime.done();

			// リスト表示
			if (action == Action::GenreSelect) {
				SharedDraw::Select<CourseGenre>()
					.setOffset(offset)
					.draw(
						genres,
						select.genre,
						[](const CourseGenre& g)->decltype(auto) {return g.getName(); }
				);
			} else if (action == Action::CourseSelect) {
				SharedDraw::Select<CourseData>()
					.setOffset(offset)
					.setDrawble([](const CourseData& c, Vec2 pos) {
      					if (c.isClear()) {
		    				FontAsset(AssetNameView(FontName::SelectMusic))(U"★").drawAt(pos + Vec2{ 37, 30 }, c.getColor());
			    		}
					})
					.draw(
						courses,
						select.course,
						[](const CourseData& c)->decltype(auto) {return c.getTitle(); }
					);
			}

			// memo
			if (pCourse) {
				SharedDraw::MemoInfo()
					.setPos(SharedDraw::MemoInfo::DefaultPos + Vec2{ 0, m_memoOffset.easeOut() })
					.draw(pCourse->getScore());
			}
			// ジャンル名表示
			::DrawTitle(pGenre);
		}
	};
	CourseSelectSceneView::CourseSelectSceneView(const CourseSelectScene* const scene) :
		m_pImpl(std::make_shared<Impl>(scene))
	{}

	void CourseSelectSceneView::update() const
	{
		m_pImpl->update();
	}

	void CourseSelectSceneView::onChangeAction() const
	{
		m_pImpl->onChangeAction();
	}

	void CourseSelectSceneView::draw() const
	{
		m_pImpl->draw();
	}
}
