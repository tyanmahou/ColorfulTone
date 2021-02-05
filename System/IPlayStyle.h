#pragma once
#include<Siv3D/Fwd.hpp>
#include<functional>
#include"UtilPP.hpp"
class Note;
class RepeatNote;
class LongNote;
class Bar;
class RepeatNote;
class RepeatEnd;

//描画モード
class IPlayStyle
{
public:
	virtual void drawJudgeLine()const {}
	virtual void drawFrame(bool red, bool blue, bool yellow, std::function<void()> drawCallback) const 
	{
		MAYBE_UNUSED(red);
		MAYBE_UNUSED(blue);
		MAYBE_UNUSED(yellow);
		MAYBE_UNUSED(drawCallback);
	}
	virtual void drawComboAndRate(int combo, float rate) 
	{
		MAYBE_UNUSED(combo);
		MAYBE_UNUSED(rate);
	};

	virtual void drawTapEffect(int type) 
	{
		MAYBE_UNUSED(type);
	};
	virtual void drawJudgeEffect(const s3d::String& str, int type)
	{
		MAYBE_UNUSED(str);
		MAYBE_UNUSED(type);
	};

	virtual void draw(const Bar& note, double count, float scrollRate)const = 0;
	virtual void draw(const Note& note, double count, float scrollRate)const = 0;
	virtual void draw(const LongNote& note, double count, float scrollRate)const = 0;
	virtual void draw(const RepeatNote& note, double count, float scrollRate)const = 0;
	virtual void draw(const RepeatEnd& note, double count, float scrollRate)const = 0;
};