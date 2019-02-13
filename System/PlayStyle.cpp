#include"PlayStyle.h"

#include"NormalStyle.h"
#include"Portrait.h"
#include<unordered_map>


PlayStyle::PlayStyle() :
	m_style(std::make_shared<NormalStyle>())
{}

void PlayStyle::setStyle(PlayStyleType type)
{
	static const std::unordered_map<PlayStyleType, std::shared_ptr<IPlayStyle>> factory
	{
		{PlayStyleType::Normal,std::make_shared<NormalStyle>()},
		{ PlayStyleType::Portrait,std::make_shared<Portrait>() },
	};

	m_style = factory.at(type);
}