#include <core/Play/Score/ScoreJudge.hpp>
#include <Siv3D.hpp>

namespace
{
	using namespace ct;
	static const s3d::HashTable<ScoreJudge, StringView> scoreMap
	{
		{ ScoreJudge::Good, U"GOOD" },
		{ ScoreJudge::Great,U"GREAT" },
		{ ScoreJudge::Perfect,U"PERFECT" },
		{ ScoreJudge::Miss, U"MISS" },
	};
}
namespace ct 
{
	s3d::StringView JudgeStr(ScoreJudge judge)
	{
		return scoreMap.at(judge);
	}
}
