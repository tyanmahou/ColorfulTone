#include"Bar.h"

bool Bar::update(double& nowCount, double& countPerFrame, Score& score, Sound& sound)
{
	return false;
}
void Bar::diffDraw(double count, float scrollRate)const 
{

	double x = 40 + count / Object::RESOLUTION * scrollRate*m_speed;

	if (x <= 0|| x>1000)
		return;
	Circle(400,300,x).drawFrame(1,1,ColorF(0,0.1));

}
