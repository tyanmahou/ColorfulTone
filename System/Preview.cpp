#include "Preview.h"

#include"AutoPlayManager.h"

void SoundPlayer::update()
{
	if (Input::KeyF11.clicked)
	{
		m_isActive ^= 1;
		m_gui.show(m_isActive);
	}
	m_gui.button(L"PlayButton").enabled = !m_scene->isPlaying()&&m_sound!=nullptr;
	m_gui.button(L"UpdateButton").enabled = !m_scene->isPlaying() && m_sound != nullptr;
	m_gui.button(L"StopButton").enabled = m_scene->isPlaying();
	m_gui.button(L"OpenButton").enabled = !m_scene->isPlaying();
	m_gui.slider(L"LevelSlider").enabled = !m_scene->isPlaying();
	//m_gui.slider(L"Slider").enabled = !m_scene->isPlaying();

	if (m_gui.button(L"PlayButton").pushed)
	{
		m_sound->play();

		m_scene->notify(L"PlayButton");
	}
	else if (m_gui.button(L"StopButton").pushed)
	{
		m_sound->pause();
		m_scene->notify(L"StopButton");

	}
	else if (m_gui.button(L"OpenButton").pushed)
	{

		m_scene->notify(L"OpenButton");

	}
	else if (m_gui.button(L"UpdateButton").pushed)
	{

		m_scene->notify(L"UpdateButton");

	}
	if (m_gui.slider(L"LevelSlider").hasChanged)
	{
		m_scene->setLevelRate(m_gui.slider(L"LevelSlider").value);
	}

	if (m_sound != nullptr)
	{
		if (m_gui.slider(L"Slider").hasChanged)
		{
			m_sound->setPosSec(m_sound->lengthSec() * m_gui.slider(L"Slider").value);
		}

		m_gui.slider(L"Slider").setValue(m_sound->streamPosSec() / m_sound->lengthSec());
	}

	ClearPrint();
	if (m_isActive)
	{
		Println(L"F1:AutoPlay");
		Println(L"F2:現在の位置から再生/停止");
		Println(L"F3:曲の初めから再生/停止");
		Println(L"F5:更新");
		Println(L"Ctrl:ハイスピの変更");


	}
	Println(L"F11:GUIの表示/非表示");

}

void Preview::loadProject(Optional<FilePath>& path)
{
	if (path.has_value())
	{
		const auto pos = m_musicGame.getSound().streamPosSec();
		const auto genre = FileSystem::BaseName(FileSystem::ParentPath(*path));

		auto assets = FileSystem::DirectoryContents(*path);
		//iniファイルがあるか検索
		for (const auto& elm : assets)
		{
			if (elm.includes(L"ini"))
			{
				SoundAsset(L"desisionSmall").playMulti(0.5);

				m_musicData.emplace(genre, *path, elm);
				m_selectLevel %= m_musicData->getNotesData().size();
				m_musicGame.init(*m_musicData, m_selectLevel, m_scrollRate);
				m_soundPlayer.setLevelName(m_musicGame.getNotesData().getLevelName());
				m_musicGame.update();
				m_musicGame.getSound().stop();
				m_soundPlayer.setSound(m_musicGame.getSound());
				break;
			}
		}

		m_musicGame.getSound().setPosSec(pos);
	}

}

void Preview::notify(const String & event)
{
	if (event == L"OpenButton")
	{
		auto path = Dialog::GetFolder(L"Music");
		if (path.has_value())
		{
			m_dirPath = path;
		}
		this->loadProject(m_dirPath);
	}
	else if (event == L"PlayButton"|| event == L"StopButton")
	{
		this->playOrStop();
	}
	else if (event == L"UpdateButton")
	{
		this->loadProject(m_dirPath);
	}

}

void Preview::update()
{
	//if (Input::KeyEscape.clicked)
	//{
	//	changeScene(L"title", 3000);
	//}
	m_soundPlayer.update();
	//プレイモード
	if (Input::KeyF1.clicked)
	{
		SoundAsset(L"desisionSmall").playMulti(0.5);
		AutoPlayManager::Instance()->m_autoPlay = !(AutoPlayManager::Instance()->m_autoPlay);
	}
	if (Input::KeyF5.clicked)
	{
		if (!m_isPlay)
			this->loadProject(m_dirPath);
	}
	if (m_musicData.has_value())
	{
		if (Input::KeyF2.clicked)
		{
			if(!m_isPlay)
			m_musicGame.getSound().play();
			else
			m_musicGame.getSound().pause();

			this->playOrStop();
		}
		if (Input::KeyF3.clicked)
		{
			if (!m_isPlay)
			{
				m_musicGame.getSound().setPosSec(0);
				m_musicGame.getSound().play();
			}
			else
				m_musicGame.getSound().pause();

			this->playOrStop();
		}
		if (Input::KeyEscape.clicked)
		{
			m_musicGame.getSound().stop();
		}
			

		if (Input::KeyRight.pressed)
		{
			auto&s = m_musicGame.getSound();
			s.setPosSample(s.streamPosSample() + 5000);
		}
		if (Input::KeyLeft.pressed)
		{
			auto&s = m_musicGame.getSound();
			auto pos = s.streamPosSample()-5000;
			if (pos <= 0)
				pos = 0;
			s.setPosSample(pos);
		}
		m_highSpeedDemo.update(m_scrollRate);
		m_musicGame.setScrollRate(m_scrollRate);

		if (m_isPlay)
		{
			m_musicGame.update();

			if (m_musicGame.isFinish())
			{
				m_musicGame.getSound().stop();
				this->playOrStop();
			}
		}
		else
		{
			m_musicGame.synchroCount(m_count);
		}
	}
}

