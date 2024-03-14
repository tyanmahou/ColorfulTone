#include <core/Play/PlayStyle/PlayStyle.hpp>

#include <core/Play/PlayStyle/Normal/NormalStyle.hpp>
#include <core/Play/PlayStyle/Portrait/PortraitStyle.hpp>
#include <core/Play/PlayStyle/NormalArc/NormalArcStyle.hpp>
#include <core/Play/PlayStyle/Homography/HomographyStyle.hpp>
#include <core/Play/PlayStyle/Landscape/LandscapeStyle.hpp>
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
           { PlayStyleType::Portrait,  [] { return std::make_unique<PortraitStyle>(); }},
           { PlayStyleType::NormalArc,  [] { return std::make_unique<NormalArcStyle>(); }},
           { PlayStyleType::Homography,  [] { return std::make_unique<HomographyStyle>(); }},
           { PlayStyleType::Landscape,  [] { return std::make_unique<LandscapeStyle>(); }},
        };
        auto it = factory.find(type);
        if (it == factory.end()) {
            m_style = std::make_unique<NormalStyle>();
        } else {
            m_style = it->second();
        }
    }
}