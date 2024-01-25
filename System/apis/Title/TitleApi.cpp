#include <apis/Title/TitleApi.hpp>
#include <apis/ApiEndPoints.hpp>
#include <utils/Http/RestRequestor.hpp>
#include <commons/Game/Game.hpp>
#include <Siv3D.hpp>

namespace ct
{
    bool TitleApi::CheckVersion()
    {
        auto httpRequest = RestRequestor::Post(Api::TitleVersion);
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
