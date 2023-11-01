#include <apis/Title/TitleApi.hpp>
#include <apis/ApiEndPoints.hpp>
#include <utils/Http/RestRequestor.hpp>

namespace ct
{
    bool TitleApi::CheckVersion()
    {
        auto httpRequest = RestRequestor(Api::TitleVersion);
        if (!httpRequest) {
            return false;
        }
        const auto& response = httpRequest.getResponse();
        const auto version = response[U"version"].getString();
        if (Version(version) > Game::Version) {
            System::MessageBoxOK(U"最新のバージョンは" + version + U"です。");
        }
        return true;
    }
}
