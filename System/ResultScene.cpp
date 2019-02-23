#include "ResultScene.hpp"
#include "Useful.hpp"

#include"ResultTweet.h"

namespace
{
}
class ResultScene::Model
{
private:
	std::shared_ptr<GameData> m_data;
	float m_clearRate;
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
		auto& music = m_data->m_nowMusics;
		return
			music.getMusicName() + L"/"
			+ music.getNotesData()[m_data->m_selectLevel].getLevelName() + L"で"
			+ Format(m_clearRate) + L"%達成\n#ColorfulTone";
	}
public:

	Model() {}

	void setData(std::shared_ptr<GameData> data)
	{
		m_data = data;
	}

	void update()
	{
		if (m_tweet.update())
		{
			return;
		}
		if (Input::KeyT.clicked)
		{
			m_tweet.tweet(
				this->getTweetText()
			);
		}
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
}

void ResultScene::finally()
{
	SoundAsset(L"result").stop(1s);
}

void ResultScene::update()
{
	m_model->update();

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
	return m_data->m_nowMusics.getNotesData()[m_data->m_selectLevel];
}
