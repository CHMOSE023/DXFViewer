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

        float zoom = app->m_camera.GetZoom();
        float ndc_dx = static_cast<float>(dx) * (2.0f / app->m_winWidth) * zoom;
        float ndc_dy = static_cast<float>(dy) * (2.0f / app->m_winHeight) * zoom;

        glm::vec3 pos = app->m_camera.GetPosition();
        pos.x -= ndc_dx * (app->m_winWidth / app->m_winHeight); // 保持比例
        pos.y += ndc_dy;
        app->m_camera.SetPosition(pos);
    }
}

void Application::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    Application* app = GetWindow(window);
    app->m_winWidth = width;
    app->m_winHeight = height;

    float aspect = static_cast<float>(width) / static_cast<float>(height);
    float zoom = app->m_camera.GetZoom();

    float viewWidth = 10.0f * zoom;
    float viewHeight = viewWidth / aspect;

    app->m_camera.SetProjection(-viewWidth / 2, viewWidth / 2, -viewHeight / 2, viewHeight / 2);
}

void Application::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Application* app = GetWindow(window);
    float zoom = app->m_camera.GetZoom();

    // 缩放速度和范围控制
    zoom -= static_cast<float>(yoffset) * 0.1f;
    zoom = std::clamp(zoom, 0.1f, 10.0f);

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

    m_pWindow = glfwCreateWindow(width, height, title, NULL, NULL);
    m_winWidth = width;
    m_winHeight = height;

    if (!m_pWindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

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
    float aspect = static_cast<float>(width) / height;
    float viewWidth = 10.0f;
    float viewHeight = viewWidth / aspect;
    m_camera.SetProjection(-viewWidth / 2, viewWidth / 2, -viewHeight / 2, viewHeight / 2);

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
        glClearColor(0.1f, 0.3f, 0.3f, 1.0f);

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
