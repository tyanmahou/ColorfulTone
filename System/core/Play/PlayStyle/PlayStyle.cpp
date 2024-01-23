#include <core/Play/PlayStyle/PlayStyle.hpp>

// TODO
#include <core/Play/PlayStyle/Normal/NormalStyle.hpp>
//#include"Portrait.h"
#include <core/Play/PlayStyle/NormalArc/NormalArcStyle.hpp>
#include <Siv3D.hpp>

namespace ct
{
    PlayStyle::PlayStyle() :
        m_style(std::make_unique<NormalStyle>())
    {}
    PlayStyle::~PlayStyle()
    {}

    void PlayStyle::setStyle(PlayStyleType type)
    {
        static const HashTable<PlayStyleType, std::function<std::unique_ptr<IPlayStyle>()>> factory
        {
           { PlayStyleType::Normal,  [] { return std::make_unique<NormalStyle>(); }},
           { PlayStyleType::Portrait,  [] { return std::make_unique<NormalStyle>(); }},
           { PlayStyleType::NormalArc,  [] { return std::make_unique<NormalArcStyle>(); }},
        };
        auto it = factory.find(type);
        if (it == factory.end()) {
            m_style = std::make_unique<NormalStyle>();
        } else {
            m_style = it->second();
        }
    }
}