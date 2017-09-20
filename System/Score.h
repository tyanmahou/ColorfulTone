#pragma once

enum class SpecialResult :unsigned int {
	None,
	Full_Combo,
	All_Perfect,
};

bool operator > (SpecialResult& l, SpecialResult& r)
{
	return static_cast<unsigned int>(l) > static_cast<unsigned int>(r);
}

struct Score
{
	enum  Judge 
	{
		Perfect,
		Great,
		Good,
		Miss,

		TERM,
	};
	unsigned int	m_currentCombo;
	unsigned int m_maxCombo;
	std::array<unsigned int, Judge::TERM> m_judgeCount{0};

	Score():m_currentCombo(0),m_maxCombo(0){}

};

