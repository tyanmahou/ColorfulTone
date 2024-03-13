#pragma once
#include <core/Play/PlayStyle/Normal/NormalStyle.hpp>

namespace ct
{
	// 通常 アーク
	class NormalArcStyle : public NormalStyle
	{
		void draw(const Note& note, double count, double scrollRate)const override;

		void draw(const LongNote& note, double count, double scrollRate)const override;

		void draw(const RepeatNote& note, double count, double scrollRate)const override;

		void draw(const RepeatEnd& note, double count, double scrollRate)const override;
	};
}

