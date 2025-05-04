#include "../../src/App/DXFViewer.h"
int main() {
	DXFViewer* app = new DXFViewer();
	app->Initialize(800, 600, "DxfViewer");	 
	app->Run();
} 