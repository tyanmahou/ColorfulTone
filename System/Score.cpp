#include"Score.h";

bool operator > (SpecialResult& l, SpecialResult& r)
{
	return static_cast<unsigned int>(l) > static_cast<unsigned int>(r);
}