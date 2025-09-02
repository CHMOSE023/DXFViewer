#include <iostream> 
#include "../../src/Dxf/DxfInterface.h"
#include "../../src/Dxf/DxfRW.h"
#ifdef _WIN32
#include <windows.h> 
#endif 
int main()
{  
#ifdef _WIN32 
    SetConsoleOutputCP(CP_UTF8);
#endif 
    std::cout << "Hello, world!  ss 中文" << std::endl; 
	const char* filename = "./Dxf/line.dxf";  
    DxfRW dxf(filename);
	DxfInterface* dxfInterface = new DxfInterface();
    dxf.Read(dxfInterface,false);
    return 0;
}
