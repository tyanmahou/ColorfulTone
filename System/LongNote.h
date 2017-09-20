#pragma once
#include"Note.h"
class LongNote :public Object
{
private:


	const int m_type;
	String m_textureName;
	double m_textureAngle;
	Color m_color;
protected:
	std::shared_ptr<Note> m_parent;
	std::function<bool(void)> m_judge;
	const double m_scrollSpeed;
	void perfect(Score& score);
	void miss(Score& score);

public:
	LongNote(int type, double firstCount,double speed,std::shared_ptr<Note>& parent);
	virtual ~LongNote() = default;
	virtual bool update(double& nowCount, double& countPerFrame, Score& score, Sound& sound)override;
	virtual void diffDraw(double count, float scrollRate)const override;
};
