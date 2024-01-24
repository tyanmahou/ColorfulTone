#include <scenes/Scene/Result/ResultScene.hpp>
#include <core/Data/Score/ResultRank.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace
{	
	using namespace ct;

	// スコアの更新　newRecordのばあいtrue
	bool UpdateScore(const ScoreModel& score, const NotesData& notes)
	{
		const uint32 musicIndex = notes.getMusic()->getIndex();
		const uint32 notesIndex = notes.getIndex();
		NotesData& srcNotes = Game::Musics()[musicIndex][notesIndex];

		ScoreModel srcScore = srcNotes.getScore();
		bool isNewRecord = false;
		if (score.isClear && !srcScore.isClear)
		{
			srcScore.isClear = true;
			isNewRecord = true;
		}
		if (score.specialResult > srcScore.specialResult)
		{
			srcScore.specialResult = score.specialResult;
			isNewRecord = true;

		}
		if (score.clearRate > srcScore.clearRate)
		{
			srcScore.clearRate = score.clearRate;
			isNewRecord = true;
		}
		if (isNewRecord)
		{
			srcNotes.setScore(srcScore);
			srcNotes.saveScore(srcScore);
		}
		return isNewRecord;
	}
	// コーススコアの更新　newRecordのばあいtrue
	bool UpdateCourseScore(const CourseScore& score, const CourseData& course)
	{
		CourseData& srcCourse = Game::Courses()[course.getIndex()];

		CourseScore srcScore = srcCourse.getScore();
		bool isNewRecord = false;
		if (score.isClear && !srcScore.isClear)
		{
			srcScore.isClear = true;
			isNewRecord = true;
		}
		if (static_cast<uint8>(score.special) > static_cast<uint8>(srcScore.special))
		{
			srcScore.special = score.special;
			isNewRecord = true;
		}
		if (score.totalRate > srcScore.totalRate)
		{
			srcScore.totalRate = score.totalRate;
			isNewRecord = true;

		}
		if (score.life > srcScore.life)
		{
			srcScore.life = score.life;
			isNewRecord = true;
		}
		if (isNewRecord)
		{
			srcCourse.setScore(srcScore);
			srcCourse.saveScore(srcScore);
		}
		return isNewRecord;
	}
	String GetCourseStateTweetText(PlayCourse::State state)
	{
		if (state == PlayCourse::State::Success)
		{
			return U"に合格!!!";
		}else if (state == PlayCourse::State::Failure)
		{
			return U"に不合格…";
		}
		return U"をプレイ中";
	}
}
namespace ct
{
	class ResultScene::Model
	{
	private:
		GameData* m_data;

		ScoreModel m_score;
		bool m_isNewRecord = false;

	public:

		Model() {}

		void setData(GameData& data)
		{
			m_data = &data;
		}

		void saveScore()
		{
			const NotesData& notes = m_data->m_nowNotes;

			m_score = ResultRank::CalcScore(m_data->m_resultScore, notes.getTotalNotes());

			// autoのばあいセーブしない
			if (AutoPlayManager::IsAutoPlay()) {
				return;
			}
			m_isNewRecord = ::UpdateScore(m_score, notes);
			// コース
			auto& course = m_data->m_course;
			if (course.isActive()) {
				const float life = ResultRank::CalcLifeRate(m_data->m_resultScore);
				course.updateScoreAndState(m_score.clearRate, life);

				if (course.isEnd()) {
					::UpdateCourseScore(course.getScore(), course.getCourse());
				}
			}
		}
		const ScoreModel& getScore()const
		{
			return m_score;
		}
		bool isNewRecord()const
		{
			return m_isNewRecord;
		}
	};

	ResultScene::ResultScene(const InitData& init) :
		ISceneBase(init),
		m_model(std::make_shared<Model>()),
		m_view(this)
	{
		SoundManager::PlayBgm(U"result", 1s);
		m_model->setData(getData());

		// スコアのセーブ
		m_model->saveScore();
		m_view.init();
	}
	void ResultScene::finally()
	{
		SoundManager::StopBgm(U"result", 1s);
		if (getData().m_toScene == SceneName::Course) {
			getData().m_course.next();
		}
	}

	void ResultScene::update()
	{
		if (PlayKey::Start().down() || PlayKey::BigBack().down()) {
			SoundManager::PlaySe(U"desisionLarge");
			if (getData().m_course.isActive()) {
				if (getData().m_course.isEnd()) {
					this->changeScene(SceneName::CourseSelect, 1000);
				} else {
					this->changeScene(SceneName::Course, 1000);
				}
			} else {
				this->changeScene(SceneName::Select, 1000);
			}
		}
		if (KeyF10.down()) {
			auto& selectMusic = *getData().m_nowNotes.getMusic();
			bool isFavorite = !selectMusic.isFavorite();
			auto index = selectMusic.getIndex();
			auto& sourceMusic = Game::Musics()[index];
			sourceMusic.saveFavorite(isFavorite);
			SoundManager::PlaySe(U"desisionSmall");
		}
		m_view.update();
	}

	void ResultScene::draw() const
	{
		m_view.draw();

		SceneInfo::Draw(U"F10:お気に入り　Enter:戻る");

		if (AutoPlayManager::IsAutoPlay()) {
			PutText(U"AutoPlay", Arg::center = Vec2{Scene::CenterF().x, 40});
		}
	}

	void ResultScene::drawFadeIn(double t) const
	{
		this->draw();
		FadeIn(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
	}

	void ResultScene::drawFadeOut([[maybe_unused]]double t) const
	{
		this->draw();
	}

	const NotesData& ResultScene::getNotes() const
	{
		return getData().m_nowNotes;
	}

	const Score& ResultScene::getResult() const
	{
		return getData().m_resultScore;
	}

	const ScoreModel& ResultScene::getScore() const
	{
		return m_model->getScore();
	}

	bool ResultScene::isNewRecord() const
	{
		return m_model->isNewRecord();
	}

	const PlayCourse& ResultScene::getPlayCourse()const
	{
		return getData().m_course;
	}
}