#pragma once

enum class Judge 
{
	Perfect =4,
	Great = 8,//9,
	Good = 12,
};

double JudgeRange(double countPerFrame, Judge judge);
