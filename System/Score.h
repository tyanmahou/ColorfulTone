#pragma once
#include<array>
#include<Siv3D/Fwd.hpp>
#include<Siv3D/Array.hpp>

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
	std::array<s3d::uint32, Judge::TERM> m_judgeCount{0};
	s3d::Array<Judge> m_judgeHistory;
	// ライフ(桁落ちの対策のため整数型)
	s3d::int32 m_initLife;
	s3d::int32 m_life;
	s3d::Array<s3d::int32> m_lifeHistory;

	Score();
	void add(Judge judge);
};

struct ScoreModel
{
	bool isClear = 0;
	SpecialResult specialResult = SpecialResult::None;
	float clearRate = 0.0f;
};

