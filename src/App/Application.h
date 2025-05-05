#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Core/TimeStamp.h"
#include "../Core/OrthoCamera.h"
class Application
{
public:
	Application();
	virtual ~Application();
	virtual void  Initialize(int width, int height, const char* title);  // 1.��ʼ��
	virtual void  Run();	  
	virtual void  Startup();        // 1.׼������
	virtual void  Render();         // 2.��Ⱦ����
	virtual void  Shutdown();       // 3.�ر�

private:
	// ϵͳ�ص�����
	static void  ErrorCallback(int error, const char* description);
	static void  KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void  MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void  CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void  WindowSizeCallback(GLFWwindow* window, int width, int height);
	static void  ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	static Application* GetWindow(GLFWwindow* window); 
 

protected:
	int           m_winWidth = 0;
	int	          m_winHeight = 0;
	GLFWwindow* m_pWindow = nullptr;
	OrthoCamera   m_camera;        // ��� 

	double m_LastCursorX = 0.0;
	double m_LastCursorY = 0.0;
	bool m_MiddleButtonPressed = false;

};

