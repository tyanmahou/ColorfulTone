#include <Siv3D.hpp> 
#include "NotesViewerApp.hpp"

void Main()
{
	auto app = std::make_unique<ct::NotesViewerApp>();
	app->run();
}
