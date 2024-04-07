#include <scenes/Scene/CourseSelect/CourseSelectSceneView.hpp>
#include <scenes/Scene/CourseSelect/CourseSelectScene.hpp>
#include <Useful.hpp>
#include <scenes/Scene/Config/ConfigMain.hpp>
#include <Siv3D.hpp>

namespace
{
	using namespace ct;
	using Action = CourseSelectScene::Action;

	constexpr double yOffset = 0;

	void DrawBG()
	{
		const TextureRegion tex = TextureAsset(U"memo2")(75, 22, 400, 110);
		for (size_t index = 0; index < 4; ++index)
		{
			tex
				.draw(10, static_cast<double>(index * 115 + 95) + yOffset)
				.drawFrame(0, 1, Color(224, 209, 173));
		}
	}
	void DrawMusicInfo(double y, Texture texture, const s3d::ColorF& jucketColor, const s3d::String& title, const s3d::String& detail)
	{
		if (texture) {
			const Vec2 jacketPos{ 70, y + 60 };
			constexpr Vec2 jacketSize{ 100.0,100.0 };
			RectF(jacketPos - jacketSize / 2.0, jacketSize).draw(jucketColor);
			texture
				.resized(jacketSize)
				.rotated(Math::ToRadians(-7.0))
				.drawAt(jacketPos);
		}

		if (title) {
			const FontAsset font16b(FontName::SelectMusic);
			ContractionDrawbleString(
				font16b(title),
				{ 130 ,y + 15 },
				270,
				Palette::Black,
				false
			);
		}
		if (detail) {
			const FontAsset font12(FontName::Bpm);
			ContractionDrawbleString(
				font12(detail),
				{ 130 ,y + 45 },
				270,
				Palette::Black,
				false
			);
		}
	}
	void DrawMusicInfo([[maybe_unused]]size_t index, s3d::int32 y, const CourceEntry& entry)
	{
		if (!entry.canPlay()) {
			RectF(10, y + 5, 400, 110).draw(ColorF(0, 0.1));
		}
		DrawMusicInfo(
			y,
			entry.getJucketTexture(),
			entry.getJucketColor(),
			entry.getJucketTitle(),
			entry.getJucketDetail()
		);
		if (!entry.canPlay()) {
			TextureAsset(U"warn_icon")
				.resized(30,30)
				.draw(Vec2{10, static_cast<double>(y + 5)});
		}
	}

	void DrawCourseMusics(const CourseData* pCourse, size_t page)
	{
		if (!pCourse)
		{
			return;
		}
		size_t entrySize = pCourse->getEntrySize();
		for (size_t i = 0; (i < 4 && page * 4 + i < entrySize); ++i) {
			size_t entryIndex = page * 4 + i;
			::DrawMusicInfo(i, static_cast<int32>(i * 115 + 90 + yOffset), (*pCourse)[entryIndex]);
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
				size_t page = m_pScene->entryPage();
				::DrawCourseMusics(pCourse, page);

				if (pCourse) {
					size_t entrySize = pCourse->getEntrySize();
					size_t pageSize = entrySize == 0 ? 1 : (entrySize - 1) / 4 + 1;
					if (pageSize > 1) {
						const FontAsset font16b(FontName::SelectMusic);
						font16b(U"{}/{}"_fmt(page + 1, pageSize)).draw(
							Arg::bottomRight = Vec2{400, 95},
							Palette::Black
						);
					}
				}
			}

			const s3d::int32 moveSelect = m_pScene->getMoveSelect();
			static EasingAB<double> easingAnime(0.0, -30.0, Easing::Linear, 100);
			if (moveSelect) {
				easingAnime.reset();
				easingAnime.start();
			}
			const double offset = easingAnime.isMoving() ?
				easingAnime.easeInOut() :
				easingAnime.getB();

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
    					if (c.isMainClear()) {
							FontAsset(AssetNameView(FontName::SelectMusic))(U"★").drawAt(pos + Vec2{ 37, 30 }, c.getStarColor());
						} else if (c.isClear()) {
		    				FontAsset(AssetNameView(FontName::SelectMusic))(U"☆").drawAt(pos + Vec2{ 37, 30 }, c.getStarColor());
						} else if (c.isLifeClear()) {
							FontAsset(AssetNameView(FontName::SelectMusic))(U"-").drawAt(pos + Vec2{ 37, 30 }, c.getStarColor());
						}
					})
					.setColorCallBack([](const CourseData& c) {
						return c.getColor();
					})
					.draw(
						courses,
						select.course,
						[](const CourseData& c)->decltype(auto) {return c.getTitle(); }
					);
			}


			//SharedDraw::JacketInfo jacketInfo;
			//jacketInfo.drawLabel();

			// memo
			if (pCourse) {
				SharedDraw::MemoInfo()
					.setPos(SharedDraw::MemoInfo::DefaultPos + Vec2{ 0, m_memoOffset.easeOut() })
					.draw(pCourse->getScore());
			}
			// ジャンル名表示
			::DrawTitle(pGenre);

			if (!m_pScene->getConfig().isActive()) {
				// SharedDraw::DrawPlayContextHeader();

				SharedDraw::DrawPlayOptionSets();
			}
			// コンフィグ
			m_pScene->getConfig().drawWithBack();
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
