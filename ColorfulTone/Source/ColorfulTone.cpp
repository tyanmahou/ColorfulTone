#include"ColorfulTone.h"
#include"Game.h"
#include"Setting.hpp"
namespace
{
	bool CheckVersion()
	{
		const FilePath url = Setting::HOMEPAGE_URL+L"Version.txt";

		if (!Internet::IsConnected()) // インターネット接続をチェック
		{
			return false;
		}

		ByteArray reader;
		HTTPClient client;
		if (client.download(url, reader))
		{
			String str = TextReader(std::move(reader)).readAll();
			if (str != Game::Version)
				MessageBox::Show(L"最新のバージョンは" + str + L"です。");

			return true;
		}
		return false;
	}
}

bool ColorfulTone::update()
{
	return Game::UpdateAndDraw();
}

ColorfulTone::ColorfulTone():
	AppBase(L"ColorfulTone")
{

	Profiler::EnableWarning(false);

	//バージョンチェック
	::CheckVersion();

}

ColorfulTone::~ColorfulTone()
{}
