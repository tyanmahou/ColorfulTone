#pragma once
#include<array>
#include<Siv3D/Fwd.hpp>

enum class SpecialResult : unsigned int
{
	None,
	Full_Combo,
	All_Perfect,
};
bool operator > (SpecialResult& l, SpecialResult& r);
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
	s3d::uint32	m_currentCombo;
	s3d::uint32 m_maxCombo;
	std::array<s3d::int32, Judge::TERM> m_judgeCount{0};

	Score():
		m_currentCombo(0),
		m_maxCombo(0)
	{}
};

struct ScoreModel
{
	bool isClear = 0;
	SpecialResult specialResult = SpecialResult::None;
	float clearRate = 0.0f;
};

