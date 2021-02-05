#pragma once

#include<Siv3D/Fwd.hpp>
#include<memory>

///<summary>
///リザルトを共有する(twitter)
///</summary>
class ResultTweet
{
	class CTweet;
public:
	ResultTweet();

	bool update();
	void tweet(const s3d::String& text);

private:
	
	std::shared_ptr<CTweet> m_pImpl;
};
