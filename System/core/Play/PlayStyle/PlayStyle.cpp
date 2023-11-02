#include <core/Play/PlayStyle/PlayStyle.hpp>

// TODO
//#include"NormalStyle.h"
//#include"Portrait.h"
//#include"NormalArcStyle.hpp"
#include<unordered_map>

namespace ct
{
    PlayStyle::PlayStyle() :
        m_style(nullptr)
        //m_style(std::make_unique<NormalStyle>())
    {}
    PlayStyle::~PlayStyle()
    {}

    void PlayStyle::setStyle(PlayStyleType type)
    {
        static const std::unordered_map<PlayStyleType, std::function<std::unique_ptr<IPlayStyle>()>> factory
        {
            //{PlayStyleType::Normal,std::make_shared<NormalStyle>()},
            //{ PlayStyleType::Portrait,std::make_shared<Portrait>() },
            //{ PlayStyleType::NormalArc,std::make_shared<NormalArcStyle>() },
        };

        m_style = factory.at(type)();
    }
}