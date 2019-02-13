#pragma once
#include"IPlayStyle.h"

//tate
class Portrait : public IPlayStyle
{

	void drawFrame(bool red, bool blue, bool yellow, std::function<void()> drawCallback) const override;
	void drawJudgeLine()const override;
	void drawComboAndRate(int combo, float rate);

	void drawTapEffect(int type);
	void drawJudgeEffect(const s3d::String& str, int type);

	void draw(const Bar& note, double count, float scrollRate)const override;

	void draw(const Note& note, double count, float scrollRate)const override;

	void draw(const LongNote& note, double count, float scrollRate)const override;

	void draw(const RepeatNote& note, double count, float scrollRate)const override;

	void draw(const RepeatEnd& note, double count, float scrollRate)const override;
};
