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
        // ��ӡ����Ĳ���
        std::cout << "Camera Parameters:" << std::endl;

        // ��ӡ�����λ��
        const glm::vec3& position = m_camera.GetPosition();
        std::cout << "Position: ("
            << position.x << ", "
            << position.y << ", "
            << position.z << ")" << std::endl;

        // ��ӡ�������ת�Ƕ�
        std::cout << "Rotation: " << m_camera.GetRotation() << " degrees" << std::endl;

        // ��ӡ���������ֵ
        std::cout << "Zoom: " << m_camera.GetZoom() << std::endl;

        // ��ӡ��ͼ����
        const glm::mat4& viewMatrix = m_camera.GetViewMatrix();
        std::cout << "View Matrix: " << glm::to_string(viewMatrix) << std::endl;

        // ��ӡͶӰ����
        const glm::mat4& projectionMatrix = m_camera.GetProjectionMatrix();
        std::cout << "Projection Matrix: " << glm::to_string(projectionMatrix) << std::endl;

        // ��ӡ��ͼͶӰ����
        const glm::mat4& viewProjectionMatrix = m_camera.GetViewProjectionMatrix();
        std::cout << "ViewProjection Matrix: " << glm::to_string(viewProjectionMatrix) << std::endl;
	}; 
	virtual void  Shutdown()
	{
		printf("Shutdown\r\n");
	};

private:

};

