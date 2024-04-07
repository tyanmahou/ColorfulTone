#pragma once
#include <Siv3D/Types.hpp>

namespace ct::SharedLogic
{
	s3d::int32 MoveSelectH();
	s3d::int32 MoveSelectV();


	s3d::int32 MoveSelectV(bool& playSe);

	bool ChangeAutoPlay();
	bool ChangePracticePlay();

	bool ChangeLifeGauge();
	bool ChangeRandomNoteType();
	bool ChangePlayStyle();
}
