#pragma once
#include"Note.h"
#include<memory>
class LongNote :public Object
{
private:
	const NoteType m_type;
	s3d::Color m_color;
protected:
	std::shared_ptr<Note> m_parent;
	std::function<bool(void)> m_judge;
	const double m_scrollSpeed;
	void perfect();
	void miss();

public:
	LongNote(int type, double firstCount,double speed,std::shared_ptr<Note>& parent);
	virtual ~LongNote() = default;
	virtual bool update(const double& nowCount,const double& countPerFrame)override;
	virtual void diffDraw(double count, float scrollRate)const override;
	const double& getSpeed()const { return m_scrollSpeed; }
	const  NoteType getType()const { return m_type; }

	const s3d::Color& getColor()const { return m_color; }
	std::shared_ptr<Note> getParent()const
	{
		return m_parent;
	}
};
