#include <core/Play/PlayBG/PlayBGFactory.hpp>

namespace ct
{
    std::shared_ptr<IPlayBG> PlayBGFactory::CreateBG(BGType type)
    {
		// TODO
		//switch (type) {
		//case BGType::Default:
		//	return std::make_shared<DefaultBG>();
		//case BGType::Blur:
		//	return std::make_shared<BlurBG>();
		//case BGType::Black:
		//	return std::make_shared<SimpleBG>(Palette::Black);
		//case BGType::White:
		//	return std::make_shared<SimpleBG>();
		//default:
		//	break;
		//}

		//return std::make_shared<DefaultBG>();
		return nullptr;
	}
}
