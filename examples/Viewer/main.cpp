#include "../../src/App/DXFViewer.h"
#ifdef _WIN32
#include <windows.h> 
#endif 
int main() {
#ifdef _WIN32 
	//SetConsoleOutputCP(CP_UTF8);
#endif 
	DXFViewer* app = new DXFViewer();
	app->Initialize(800, 600, "DxfViewer");	 
	app->Run();
} 