#include"AppBase.h"
#include"Game.h"
#include"FileLoad.h"
#include"MusicSelectScene.h"

AppBase::AppBase(const String & appName, Size windowSize)
{
	Window::SetTitle(appName);
	Window::Resize(windowSize);
	ham::ScalableWindow::Setup(windowSize);
	System::SetExitEvent((Input::KeyAlt + Input::KeyF4).clicked | WindowEvent::CloseButton);

	//ゲームコンフィグの初期化
	//キー入力やハイスピ等
	Game::Instance()->m_config.init();
}

AppBase::~AppBase()
{
	Game::Instance()->m_config.save();
}

void AppBase::mainLoop()
{
	static auto pLoadResult = &FileLoad::m_loadResult;
	static auto pAudioResult = &MusicSelectScene::m_audioResult;

	auto& mutex = GetMutex();
	while (1)
	{
		mutex.lock();
		if (!System::Update())
		{
			mutex.unlock();
			break;
		}
		mutex.unlock();

		{
			const auto transformer = ScalableWindow::CreateTransformer();

			if (!this->update())
				break;
		}
		ScalableWindow::DrawBlackBars();
	}
	pLoadResult->get();
	pAudioResult->get();
}
