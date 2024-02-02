#pragma once
#include <commons/Application/base/BaseApp.hpp>
#include <commons/SingletonManager/SingletonManager.hpp>

namespace ct
{
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
		SingletonManager m_singletonManager;
	};
}
