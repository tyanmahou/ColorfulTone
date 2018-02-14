#pragma once
#include"NotesData.h"
class HighSpeedDemo 
{
private:
	Array<std::shared_ptr<Object>> m_objects;//�f���p���ʃf�[�^

	EasingController<int> m_offset;

	Sound m_sound;

	const Rect m_bgRect;
	const Circle m_judgeCircle;

	void drawDemoNotes(const Mahou::SoundBar& bar, float scrollRate,int stencilNum)const;
public:
	HighSpeedDemo();
	~HighSpeedDemo();
	void update(float& scrollRate);
	void draw(const Mahou::SoundBar& min,const Mahou::SoundBar& max, float scrollRate)const;
};