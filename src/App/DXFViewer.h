#pragma once
#include "Application.h"
#include <stdio.h>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

class DXFViewer :public Application
{
	
public:	 
	int i = 0;
	virtual void  Startup()
	{
	
		printf("Startup\r\n");
		 
	}; 

	virtual void  Render()
	{		  
        // 打印相机的参数
        std::cout << "Camera Parameters:" << std::endl;

        // 打印相机的位置
        const glm::vec3& position = m_camera.GetPosition();
        std::cout << "Position: ("
            << position.x << ", "
            << position.y << ", "
            << position.z << ")" << std::endl;

        // 打印相机的旋转角度
        std::cout << "Rotation: " << m_camera.GetRotation() << " degrees" << std::endl;

        // 打印相机的缩放值
        std::cout << "Zoom: " << m_camera.GetZoom() << std::endl;

        // 打印视图矩阵
        const glm::mat4& viewMatrix = m_camera.GetViewMatrix();
        std::cout << "View Matrix: " << glm::to_string(viewMatrix) << std::endl;

        // 打印投影矩阵
        const glm::mat4& projectionMatrix = m_camera.GetProjectionMatrix();
        std::cout << "Projection Matrix: " << glm::to_string(projectionMatrix) << std::endl;

        // 打印视图投影矩阵
        const glm::mat4& viewProjectionMatrix = m_camera.GetViewProjectionMatrix();
        std::cout << "ViewProjection Matrix: " << glm::to_string(viewProjectionMatrix) << std::endl;
	}; 
	virtual void  Shutdown()
	{
		printf("Shutdown\r\n");
	};

private:

};

