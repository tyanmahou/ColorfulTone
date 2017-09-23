#include"AppBase.h"
#include"Game.h"
#include"FileLoad.h"
#include"MusicSelectScene.h"
namespace
{
	enum class WindowSize
	{
		Hurf,
		Default,
		FullScreen,
	};
	void ChangeWindowSize(WindowSize windowSize)
	{
		if (windowSize != WindowSize::FullScreen)
		{
			Window::SetFullscreen(false, Window::BaseSize());
		}
		switch (windowSize)
		{
		case WindowSize::Hurf:
			Window::Resize(Window::BaseSize() / 2);
			break;
		case WindowSize::Default:
			Window::Resize(Window::BaseSize());
			break;
		case WindowSize::FullScreen:
			Window::SetFullscreen(true, Window::BaseSize());
			break;
		}
	}
	void ChangeWindowSize()
	{
		static WindowSize ws = WindowSize::Default;

		switch (ws)
		{
		case WindowSize::Hurf:
			ws = WindowSize::Default;
			break;
		case WindowSize::Default:
			ws = WindowSize::FullScreen;
			break;
		case WindowSize::FullScreen:
			ws = WindowSize::Hurf;
			break;
		default:
			break;
		}

		::ChangeWindowSize(ws);
	}
}
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

		if (Input::KeyF4.clicked)
		{
			::ChangeWindowSize();
		}
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
