#include"ColorfulTone.h"
#include"Game.h"
#include"Setting.hpp"
namespace
{
	bool CheckVersion()
	{
		const FilePath url = Setting::API_ENDPOINT + L"title/version";

		if (!Internet::IsConnected()) // インターネット接続をチェック
		{
			return false;
		}

		ByteArray reader;
		HTTPClient client;
		if (client.download(url, reader))
		{
			JSONReader response(std::move(reader));
			if (!response || response[L"status"].getOr<String>(L"failed") == L"failed")
			{
				return false;
			}
			const auto version = response[L"value.version"].getString();
			if (version != Game::Version)
			{
				MessageBox::Show(L"最新のバージョンは" + version + L"です。");
			}
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
