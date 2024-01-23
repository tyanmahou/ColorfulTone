#include <core/Play/PlayBG/PlayBGFactory.hpp>
#include <core/Play/PlayBG/Default/DefaultBG.hpp>
#include <core/Play/PlayBG/Blur/BlurBG.hpp>
#include <core/Play/PlayBG/Simple/SimpleBG.hpp>
#include <core/Play/PlayBG/Monochro/MonochroBG.hpp>

namespace ct
{
    std::shared_ptr<IPlayBG> PlayBGFactory::CreateBG(BGType type)
    {
        switch (type) {
        case BGType::Default:
            return std::make_shared<DefaultBG>();
        case BGType::Blur:
            return std::make_shared<BlurBG>();
        case BGType::Black:
            return std::make_shared<SimpleBG>(s3d::Palette::Black);
        case BGType::White:
            return std::make_shared<SimpleBG>();
        case BGType::NoneBlack:
            return std::make_shared<MonochroBG>(s3d::Palette::Black);
        case BGType::NoneWhite:
            return std::make_shared<MonochroBG>();
        default:
            break;
        }
        return std::make_shared<DefaultBG>();
    }
}
