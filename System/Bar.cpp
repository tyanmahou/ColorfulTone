#include "Bar.h"
#include "PlayStyle.h"
#include "UtilPP.hpp"
bool Bar::update(const double& nowCount,const double& countPerFrame)
{
	MAYBE_UNUSED(nowCount);
	MAYBE_UNUSED(countPerFrame);
	return true;
}
void Bar::diffDraw(double count, float scrollRate)const 
{
	PlayStyle::Instance()->draw(*this, count, scrollRate);
}
