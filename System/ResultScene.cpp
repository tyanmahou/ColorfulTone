#include "ResultScene.hpp"
#include "ResultRank.h"
#include "Useful.hpp"
#include "AutoPlayManager.h"
#include "ResultTweet.h"
namespace
{	
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
			return L"に合格!!!";
		}else if (state == PlayCourse::State::Failure)
		{
			return L"に不合格…";
		}
		return L"をプレイ中";
	}
}
class ResultScene::Model
{
private:
	std::shared_ptr<GameData> m_data;

	ScoreModel m_score;
	bool m_isNewRecord = false;
	ResultTweet m_tweet;
	String getTweetText()const
	{
		auto& course = m_data->m_course;
		if (course.isActive())
		{
			return course.getCourse().getTitle()
				+ ::GetCourseStateTweetText(course.getState()) + L"/"
				+ Format(L"{:.2f}%達成\n#ColorfulTone"_fmt, course.getScore().totalRate);
		}
		const auto& music = *m_data->m_nowNotes.getMusic();
		return
			music.getMusicName() + L"/"
			+ m_data->m_nowNotes.getLevelName() + L"で"
			+ Format(L"{:.2f}%達成\n#ColorfulTone"_fmt, m_score.clearRate);
	}
public:

	Model() {}

	void setData(std::shared_ptr<GameData> data)
	{
		m_data = data;
	}

	void saveScore()
	{
		const NotesData& notes = m_data->m_nowNotes;

		m_score = ResultRank::CalcScore(m_data->m_resultScore, notes.getTotalNotes());

		// autoのばあいセーブしない
		if (AutoPlayManager::IsAutoPlay())
		{
			return;
		}
		m_isNewRecord = ::UpdateScore(m_score, notes);
		// コース
		auto& course = m_data->m_course;
		if (course.isActive())
		{
			const float life = ResultRank::CalcLifeRate(m_data->m_resultScore);
			course.updateScoreAndState(m_score.clearRate, life);

			if (course.isEnd())
			{
				::UpdateCourseScore(course.getScore(),course.getCourse());
			}
		}
	}
	bool tweetUpdate()
	{
		if (m_tweet.update())
		{
			return true;
		}
		if (Input::KeyT.clicked)
		{
			m_tweet.tweet(
				this->getTweetText()
			);
		}
		return false;
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

ResultScene::ResultScene() :
	m_model(std::make_shared<Model>()),
	m_view(this)
{}

void ResultScene::init()
{

	SoundManager::BGM::Play(L"result", 1s);
	m_model->setData(m_data);
	// スコアのセーブ
	m_model->saveScore();
	m_view.init();
}

void ResultScene::finally()
{
	SoundAsset(L"result").stop(1s);
	if (m_data->m_toScene == SceneName::Course)
	{
		m_data->m_course.next();
	}
}

void ResultScene::update()
{
	if (!m_model->tweetUpdate())
	{
		if (PlayKey::Start().clicked || PlayKey::BigBack().clicked)
		{
			SoundManager::SE::Play(L"desisionLarge");
			if (m_data->m_course.isActive())
			{
				if (m_data->m_course.isEnd())
				{
					this->changeScene(SceneName::CourseSelect, 1000);
				}
				else {
					this->changeScene(SceneName::Course, 1000);
				}
			}
			else
			{
				this->changeScene(SceneName::Select, 1000);
			}
		}
		if (Input::KeyF10.clicked) {
			auto& selectMusic = *m_data->m_nowNotes.getMusic();
			bool isFavorite = !selectMusic.isFavorite();
			auto index = selectMusic.getIndex();
			auto& sourceMusic = Game::Musics()[index];
			sourceMusic.saveFavorite(isFavorite);
			SoundManager::SE::Play(L"desisionSmall");
		}
	}
	m_view.update();
}

void ResultScene::draw() const
{
	m_view.draw();

	SceneInfo::Draw(L"T:リザルトをツイート　F10:お気に入り Enter:戻る");

	if (AutoPlayManager::IsAutoPlay())
	{
		PutText(L"AutoPlay").at(Window::Center().x, 40);
	}
}

void ResultScene::drawFadeIn(double t) const
{
	this->draw();
	FadeIn(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
}

void ResultScene::drawFadeOut(double t) const
{
	this->draw();
}

const NotesData & ResultScene::getNotes() const
{
	return m_data->m_nowNotes;
}

const Score& ResultScene::getResult() const
{
	return m_data->m_resultScore;
}

const ScoreModel& ResultScene::getScore() const
{
	return m_model->getScore();
}

bool ResultScene::isNewRecord() const
{
	return m_model->isNewRecord();
}

const PlayCourse & ResultScene::getPlayCourse()const
{
	return m_data->m_course;
}
