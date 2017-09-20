
#include"MahouSoundBeat.h"
# include <Siv3D.hpp>
#include"Preview.h"
void Main()
{
	Window::SetTitle(L"ColorfulTone-NotesViewer");
	Window::Resize(800, 600);
	System::SetExitEvent((Input::KeyAlt + Input::KeyF4).clicked | WindowEvent::CloseButton);

	//ゲームコンフィグの初期化
	//キー入力やハイスピ等
	Game::Instance()->m_config.init();

	Preview viewer;

	viewer.init();

	while (System::Update())
	{
		viewer.update();
		viewer.draw();
	}
}
