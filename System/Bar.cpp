#include"Bar.h"
#include"PlayStyle.h"
bool Bar::update(double& nowCount, double& countPerFrame, Score& score, Sound& sound)
{
	return false;
}
void Bar::diffDraw(double count, float scrollRate)const 
{
	PlayStyle::Instance()->draw(*this, count, scrollRate);
}
