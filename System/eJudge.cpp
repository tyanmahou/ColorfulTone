#include"eJudge.h"
#include<Siv3D.hpp>

double JudgeRange(double countPerFrame, Judge judge)
{
	return Abs(countPerFrame)*static_cast<double>(judge);
}
