#pragma once
#include"NotesData.h"
class HighSpeedDemo 
{
private:
	Array<std::shared_ptr<Object>> m_objects;//デモ用譜面データ

	EasingController<int> m_offset;

	Sound m_sound;

	const Rect m_bgRect;
	const Circle m_judgeCircle;
	Stopwatch m_stopwatch;
	void drawDemoNotes(const Mahou::SoundBar& bar, float scrollRate,int stencilNum)const;
public:
	HighSpeedDemo();
	~HighSpeedDemo();
	bool update(float& scrollRate);
	void draw(const Mahou::SoundBar& min,const Mahou::SoundBar& max, float scrollRate)const;
};
