
# include <Siv3D.hpp>

#include"Game.h"
#include"PlayKey.h"
#include"FileLoad.h"
#include"MusicSelectScene.h"
bool CheckVersion()
{
	const FilePath url = L"http://www11.atpages.jp/tyanmahou/game/ColorfulTone/Version.txt";

	if (!Internet::IsConnected()) // インターネット接続をチェック
	{
		return false;
	}

	ByteArray reader;
	HTTPClient client;
	if (client.download(url, reader))
	{
		String str = TextReader(std::move(reader)).readAll();
		if(str!=Game::Version)
		MessageBox::Show(L"最新のバージョンは" +str + L"です。");

		return true;
	}
	return false;
}
//#define SHADERCOMPILE

void Main()
{
	Profiler::EnableWarning(false);
	Window::SetTitle(L"ColorfulTone");
	Window::Resize(800, 600);
	System::SetExitEvent((Input::KeyAlt+Input::KeyF4).clicked|WindowEvent::CloseButton);
	//Graphics::SetBackground(Palette::White);

#ifdef SHADERCOMPILE 
	Println(Shader::Compile(L"Shaders/mainBG.hlsl", L"Shaders/mainBG2.ps", ShaderType::PS_4_0, 1));
	Println(Shader::Compile(L"Shaders/ResultBG.hlsl", L"Shaders/ResultBG3.ps", ShaderType::PS_4_0, 1));
	Println(Shader::Compile(L"Shaders/startTrans.hlsl", L"Shaders/startTrans3.ps", ShaderType::PS_4_0, 1));

#endif

	
	//バージョンチェック
	CheckVersion();
	
	//ゲームコンフィグの初期化
	//キー入力やハイスピ等
	Game::Instance()->m_config.init();

	static auto pLoadResult = &FileLoad::m_loadResult;
	static auto pAudioResult = &MusicSelectScene::m_audioResult;

	auto& mutex=GetMutex();
	while (1)
	{
		mutex.lock();
		if (!System::Update())
		{
			mutex.unlock();
			break;
		}
		mutex.unlock();

		if (!Game::Instance()->updateAndDraw())
			break;

	}
	pLoadResult->get();
	pAudioResult->get();
	Game::Instance()->m_config.save();
}
