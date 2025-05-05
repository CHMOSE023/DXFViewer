#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/// <summary>
/// 实现一个用于 2D 渲染的正交相机类。
/// 提供相机的位置、旋转、投影矩阵和视图矩阵等基本功能。
/// </summary>
class OrthoCamera
{
public:
    OrthoCamera();
    ~OrthoCamera();

public: 
    void SetView(int windowWidth, int windowHeight);
    void SetView(float fixedHeight, int windowWidth, int windowHeight);
    void SetPosition(const glm::vec3& pos);
    void SetZoom(float zoom);

    const double& GetZoom() const { return m_Zoom ;} 
    const glm::vec3& GetPosition() const { return m_Position; } 
    const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; };

private:
    void RecalculateMatrix();

    glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
    double m_Zoom = 200.0f;
    double m_FixedHeight = 1000.0f;

    int m_WindowWidth = 1280;
    int m_WindowHeight = 720;

    glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
    glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
    glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
};
