#include "NotesViewerApp.hpp"
#include <commons/Constants.hpp>
#include <commons/Game/Game.hpp>
#include <scenes/Scene/Preview/Preview.hpp>
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
		Game::Config().init();
		m_preview = std::make_unique<Preview>();
		m_preview->init();
	}
	bool NotesViewerApp::onUpdate()
	{
		this->preUpdate();
		return m_preview->updateAndDraw();
	}
	void NotesViewerApp::onShutdown()
	{
		Game::Config().save();
	}
}
