#pragma once
#include <core/Play/PlayStyle/PlayStyle.hpp>

namespace ct
{
	//通常
	class NormalStyle : public IPlayStyle
	{

		void drawFrame(bool red, bool blue, bool yellow, std::function<void()> drawCallback) const override;
		void drawJudgeLine()const override;
		void drawComboAndRate(int combo, float rate);

		void drawTapEffect(int type);
		void drawJudgeEffect(const s3d::String& str, int type);

		void draw(const Bar& note, double count, double scrollRate)const override;

		void draw(const Note& note, double count, double scrollRate)const override;

		void draw(const LongNote& note, double count, double scrollRate)const override;

		void draw(const RepeatNote& note, double count, double scrollRate)const override;

		void draw(const RepeatEnd& note, double count, double scrollRate)const override;
	};
}

