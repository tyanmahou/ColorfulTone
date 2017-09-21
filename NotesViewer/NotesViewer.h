#pragma once
#include"AppBase.h"
#include"Preview.h"
class NotesViewer:public AppBase
{
private:
	Preview m_viewer;

	bool update()override
	{
		m_viewer.update();
		m_viewer.draw();
		return true;
	}
public:
	NotesViewer():
		AppBase(L"ColorfulTone-NotesViewer")
	{
		m_viewer.init();
	}

};