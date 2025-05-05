#pragma once
#include "Application.h"
#include <stdio.h>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include "../Entity/Triangle.h"

class DXFViewer :public Application
{ 
	Triangle  m_triangle;
public:	 	 
	virtual void  Startup()
	{ 
		m_triangle.Init();
	
		printf("Startup\r\n");
		 
	}; 

	virtual void  Render()
	{		  
		m_triangle.Render(m_camera);
       
	}; 
	virtual void  Shutdown()
	{
		printf("Shutdown\r\n");
	};

private:

};

