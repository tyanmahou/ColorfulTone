#pragma once
#include <commons/Application/base/BaseApp.hpp>
#include <commons/SingletonManager/SingletonManager.hpp>
#include <scenes/Scene/Preview/Preview.hpp>

namespace ct
{
	class Preview;

	class NotesViewerApp : public BaseApp
	{
	public:
		NotesViewerApp();
		~NotesViewerApp() override;
	private:
		void onStartup() override;
		bool onUpdate() override;
		void onShutdown() override;
	private:
		Preview m_preview;
		SingletonManager m_singletonManager;
	};
}
