#pragma once
#include<Siv3D.hpp>
class AppBase
{
protected:
	virtual bool update() = 0;

public:
	AppBase(const String& appName, Size windowSize = Size{800,600});
	virtual ~AppBase();

	void mainLoop();

};
