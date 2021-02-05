#include "TitleApi.hpp"
#include "Useful.hpp"
#include "ApiEndPoint.hpp"
#include "HttpRequest.hpp"

bool TitleApi::CheckVersion()
{
	auto httpRequest = HttpRequest(Api::TitleVersion);
	if (!httpRequest) {
		return false;
	}
	const auto& response = httpRequest.getResponse();
	const auto version = response[L"version"].getString();
	if (version != Game::Version)
	{
		MessageBox::Show(L"最新のバージョンは" + version + L"です。");
	}
	return true;
}
