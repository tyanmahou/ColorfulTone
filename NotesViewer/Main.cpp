#include <Siv3D.hpp> 
#include "NotesViewerApp.hpp"

void Main()
{
	auto* app = new ct::NotesViewerApp();
	app->run();

	// NOTE: 謎のバグ対応を試す
	try {
		delete app;
	}
	catch (...) {

	}
}
