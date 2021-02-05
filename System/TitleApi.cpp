#include "TitleApi.hpp"
#include "Useful.hpp"
#include "ApiEndPoint.hpp"
#include "HttpRequest.hpp"

#include <sstream>

struct Version
{
	s3d::int32 major = 0;
	s3d::int32 minor = 0;
	s3d::int32 build = 0;

	Version() = default;
	Version(const s3d::String& version)
	{
		std::wstringstream ss(version.replace(L"Ver", L"").replace(L".", L" ").c_str());

		ss >> major >> minor >> build;
	}
};
bool operator > (const Version& a, const Version& b)
{
	return std::tie(a.major, a.minor, a.build) > std::tie(b.major, b.minor, b.build);
}
bool TitleApi::CheckVersion()
{
	auto httpRequest = HttpRequest(Api::TitleVersion);
	if (!httpRequest) {
		return false;
	}
	const auto& response = httpRequest.getResponse();
	const auto version = response[L"version"].getString();
	if (Version(version) > Version(Game::Version))
	{
		MessageBox::Show(L"最新のバージョンは" + version + L"です。");
	}
	return true;
}
