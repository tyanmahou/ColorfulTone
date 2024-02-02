#include "NotesViewerApp.hpp"
#include <commons/Constants.hpp>
#include <Siv3D.hpp>

namespace ct
{
	NotesViewerApp::NotesViewerApp() :
		BaseApp(Constants::AppName + String(U"-NotesViewer"), Constants::AppResolution, true)
	{
	}
	NotesViewerApp::~NotesViewerApp()
	{
	}
	void NotesViewerApp::onStartup()
	{
	}
	bool NotesViewerApp::onUpdate()
	{
		this->preUpdate();
		return true;
	}
	void NotesViewerApp::onShutdown()
	{
	}
}
