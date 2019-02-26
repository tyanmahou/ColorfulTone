#include "ResultScene.hpp"
#include "ResultRank.h"
#include "Useful.hpp"
#include "AutoPlayManager.h"
#include "ResultTweet.h"
#include "ScoreLoader.hpp"
#include "ScoreLoader.hpp"
namespace
{
	// newRecordのばあいtrue
	std::pair<bool, ScoreModel> UpdateScore(const ScoreModel& score, const NotesData& notes)
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
		srcNotes.setScore(srcScore);
		return { isNewRecord, srcScore };
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
		if (m_data->m_isCoursePlay)
		{
			//auto& cource = Game::Courses()[m_data->m_selectCourse];
			//return  text = cource.getTitle() +
			//	(m_passEffect == PassEffect::Pass ? L"に合格" :
			//		m_passEffect == PassEffect::None ? L"をプレイ中" :
		}
		const auto& music = *m_data->m_nowNotes.getMusic();
		return
			music.getMusicName() + L"/"
			+ m_data->m_nowNotes.getLevelName() + L"で"
			+ Format(m_score.clearRate) + L"%達成\n#ColorfulTone";
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
		const auto updated = ::UpdateScore(m_score, notes);
		m_isNewRecord = updated.first;
		if (m_isNewRecord)
		{
			ScoreLoader::Save(notes.getScorePath(), updated.second);
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
	SoundAsset(L"result").play(1s);
	m_model->setData(m_data);
	// スコアのセーブ
	m_model->saveScore();
	m_view.init();
}

void ResultScene::finally()
{
	SoundAsset(L"result").stop(1s);
}

void ResultScene::update()
{
	if (!m_model->tweetUpdate())
	{
		if (PlayKey::Start().clicked || PlayKey::BigBack().clicked)
		{
			SoundManager::SE::Play(L"desisionLarge");
			if (m_data->m_isCoursePlay)
			{
				// TODO コース
			}
			else
			{
				this->changeScene(SceneName::Select, 1000);
			}
		}
	}
	m_view.update();
}

void ResultScene::draw() const
{
	m_view.draw();

	SceneInfo::Draw(L"T:リザルトをツイート Enter: 戻る");
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
