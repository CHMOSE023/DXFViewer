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
    // 键盘事件处理逻辑
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
        }
        else if (action == GLFW_RELEASE)
        {
            app->m_MiddleButtonPressed = false;
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
}

void Application::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Application* app = GetWindow(window);
    float zoom = app->m_camera.GetZoom(); 
    if (yoffset>0)
        zoom = zoom * 1.15;
    else 
        zoom = zoom * 0.85;
 
    app->m_camera.SetZoom(zoom);
}
// 获取窗口的用户数据
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
	// 设置窗口位置为屏幕中心
    glfwSetWindowPos(m_pWindow, (mode->width- width)/2, (mode->height - height) / 2);

	// 窗口的物理尺寸
    int widthMM, heightMM;
    glfwGetMonitorPhysicalSize(primary, &widthMM, &heightMM);

	// 计算 DPI
	float dpiX = static_cast<float>(mode->width) / (static_cast<float>(widthMM) );    
    float dpiY = static_cast<float>(mode->height) / (static_cast<float>(heightMM));

    glfwShowWindow(m_pWindow);
    // 设置 glfw 回调函数
    glfwSetKeyCallback(m_pWindow, KeyCallback);
    glfwSetMouseButtonCallback(m_pWindow, MouseButtonCallback);
    glfwSetCursorPosCallback(m_pWindow, CursorPositionCallback);
    glfwSetWindowSizeCallback(m_pWindow, WindowSizeCallback);
    glfwSetScrollCallback(m_pWindow, ScrollCallback);

    glfwMakeContextCurrent(m_pWindow);

    gladLoadGL();
    glfwSwapInterval(1); // 垂直同步
    glEnable(GL_DEPTH_TEST); // 开启深度测试

    // 设置相机
    m_camera.SetView(width, height);   

    glfwSetWindowUserPointer(m_pWindow, this);
}

void Application::Run()
{
    Startup(); // 准备工作

    while (!glfwWindowShouldClose(m_pWindow))
    {
        glfwGetFramebufferSize(m_pWindow, &m_winWidth, &m_winHeight);

        glViewport(0, 0, m_winWidth, m_winHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        Render(); // 渲染数据

        glfwSwapBuffers(m_pWindow);
        glfwWaitEvents(); // 等待事件
    }

    Shutdown(); // 关闭程序
}

void Application::Startup()
{
    // 初始化数据
}

void Application::Render()
{
    
}

void Application::Shutdown()
{
    // 清理资源
}
