#include "Application.h"
#include <algorithm>
#include <iostream>

Application::Application() {}

Application::~Application()
{
    glfwDestroyWindow(m_pWindow);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void Application::ErrorCallback(int error, const char* description)
{
    std::cerr << "Error: " << description << std::endl;
}

void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // �����¼������߼�
}

void Application::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Application* app = GetWindow(window);

    if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        if (action == GLFW_PRESS)
        {
            app->m_MiddleButtonPressed = true;
            glfwGetCursorPos(window, &app->m_LastCursorX, &app->m_LastCursorY);
            
            // ����ץ�ֹ��
            // glfwSetCursor(window, glfwCreateStandardCursor(GLFW_HAND_CURSOR));
        }
        else if (action == GLFW_RELEASE)
        {
            app->m_MiddleButtonPressed = false;

            // �ָ�Ĭ�ϼ�ͷ���
            // glfwSetCursor(window, glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
        }
    }
}

void Application::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    Application* app = GetWindow(window);

    if (app->m_MiddleButtonPressed)
    {
        double dx = xpos - app->m_LastCursorX;
        double dy = ypos - app->m_LastCursorY;

        app->m_LastCursorX = xpos;
        app->m_LastCursorY = ypos;
       
        glm::vec3 pos = app->m_camera.GetPosition();

        pos.x -= (dx / app->m_camera.GetZoom() * 0.5);
        pos.y += (dy / app->m_camera.GetZoom() * 0.5);
      
		//printf("pos.x = %f, pos.y = %f  \r\n", pos.x, pos.y);
    
        app->m_camera.SetPosition(pos);
    }
}

void Application::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    Application* app = GetWindow(window);
    app->m_winWidth = width;
    app->m_winHeight = height;   
    app->m_camera.SetView(width, height);
    glViewport(0, 0, width, height);
   
}

void Application::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{

    Application* app = GetWindow(window);

    // 1. ��ȡ����ڴ����е�λ��
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // 2. �������Ļ����ת��Ϊ��������
    float ndcX = (2.0f * static_cast<float>(mouseX)) / app->m_winWidth - 1.0f;
    float ndcY = 1.0f - (2.0f * static_cast<float>(mouseY)) / app->m_winHeight;

    glm::vec4 ndcPos = glm::vec4(ndcX, ndcY, 0.0f, 1.0f);
    glm::mat4 invVP = glm::inverse(app->m_camera.GetViewProjectionMatrix());
    glm::vec4 worldPosBeforeZoom = invVP * ndcPos;

    // 3. ִ������
    float zoom = app->m_camera.GetZoom();
    if (yoffset > 0)
        zoom *= 1.15f;
    else
        zoom *= 0.85f;
    app->m_camera.SetZoom(zoom);

    // 4. ���¼�����������
    invVP = glm::inverse(app->m_camera.GetViewProjectionMatrix());
    glm::vec4 worldPosAfterZoom = invVP * ndcPos;

    // 5. �������λ�ò��䣺�������λ��
    glm::vec3 camPos = app->m_camera.GetPosition();
    glm::vec3 offset = glm::vec3(worldPosBeforeZoom - worldPosAfterZoom); 
    app->m_camera.SetPosition(camPos + glm::vec3(offset.x*0.5,offset.y*0.5,offset.z*0.5));

}
// ��ȡ���ڵ��û�����
Application* Application::GetWindow(GLFWwindow* window)
{
    void* userdata = glfwGetWindowUserPointer(window);
    return reinterpret_cast<Application*>(userdata);
}

void Application::Initialize(int width, int height, const char* title)
{
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    m_pWindow = glfwCreateWindow(width, height, title, NULL, NULL);
    m_winWidth = width;
    m_winHeight = height;

    if (!m_pWindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwDefaultWindowHints();



    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary);    
	// ���ô���λ��Ϊ��Ļ����
    glfwSetWindowPos(m_pWindow, (mode->width- width)/2, (mode->height - height) / 2);

	// ���ڵ�����ߴ�
    int widthMM, heightMM;
    glfwGetMonitorPhysicalSize(primary, &widthMM, &heightMM);

	// ���� DPI
	float dpiX = static_cast<float>(mode->width) / (static_cast<float>(widthMM) );    
    float dpiY = static_cast<float>(mode->height) / (static_cast<float>(heightMM));

    glfwShowWindow(m_pWindow);
    // ���� glfw �ص�����
    glfwSetKeyCallback(m_pWindow, KeyCallback);
    glfwSetMouseButtonCallback(m_pWindow, MouseButtonCallback);
    glfwSetCursorPosCallback(m_pWindow, CursorPositionCallback);
    glfwSetWindowSizeCallback(m_pWindow, WindowSizeCallback);
    glfwSetScrollCallback(m_pWindow, ScrollCallback);

    glfwMakeContextCurrent(m_pWindow);

    gladLoadGL();
    glfwSwapInterval(1); // ��ֱͬ��
    glEnable(GL_DEPTH_TEST); // ������Ȳ���

    // �������
    m_camera.SetView(width, height);   
        
    glfwSetWindowUserPointer(m_pWindow, this);
}

void Application::Run()
{
    Startup(); // ׼������

    while (!glfwWindowShouldClose(m_pWindow))
    {  
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Render(); // ��Ⱦ����

        glfwSwapBuffers(m_pWindow);
        glfwWaitEvents(); // �ȴ��¼�
    }

    Shutdown(); // �رճ���
}

void Application::Startup()
{
    // ��ʼ������
}

void Application::Render()
{
    
}

void Application::Shutdown()
{
    // ������Դ
}
