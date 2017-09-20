#pragma once
#include"Game.h"


///<summary>
///ƒŠƒUƒ‹ƒg‚ð‹¤—L‚·‚é(twitter)
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
