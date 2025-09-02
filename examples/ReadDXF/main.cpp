#include <iostream>  
#include "../../src/Core/DxfImport.h"
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
    
	DxfImport   importer;
    return 0;
}
