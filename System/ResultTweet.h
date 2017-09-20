#pragma once
#include"Game.h"


///<summary>
///リザルトを共有する(twitter)
///</summary>
class ResultTweet
{
	class CTweet;
public:
	ResultTweet();

	bool update();
	void tweet(const String& text);

private:
	
	std::shared_ptr<CTweet> m_pImpl;
};
