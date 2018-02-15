#pragma once
#include"Object.h"
#include"Stop.h"
#include<functional>


enum ColorIndex 
{
	Red,
	Blue,
	Yellow,
};
using NoteType = int;
class Note :public  Object 
{
private:
	const double m_scrollSpeed;

	bool m_isAnyClicked = false;
	bool m_isClicked[3];


	const NoteType m_type;	//あとでenumに？
	Color m_color;
	std::function<bool(void)> m_judge;


	void tapUpdate(Score::Judge judge, Score& score);
	void tapMiss(Score& score);
protected:

public:
	Note(const NoteType type, double firstCount, double speed);
	virtual ~Note() = default;
	virtual void init()override;
	virtual bool update(double& nowCount, double& countPerFrame, Score& score,Sound& sound)override;
	virtual void diffDraw(double count, float scrollRate)const override;
	const Color& getColor()const { return m_color; }
	const  NoteType getType()const { return m_type; }
	const double& getSpeed()const {	return m_scrollSpeed;}
	virtual bool isFirstTap()const;
};
