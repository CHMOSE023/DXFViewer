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
    OrthoCamera(); // 默认构造函数
    OrthoCamera(float left, float right, float bottom, float top); // 有参构造函数

    void SetPosition(const glm::vec3& position);
    void SetRotation(float rotation);
    void SetZoom(float zoom);  // 设置缩放
    float GetZoom() const { return m_Zoom; }

    const glm::vec3& GetPosition() const { return m_Position; }
    float GetRotation() const { return m_Rotation; }

    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    void RecalculateViewMatrix(); // 计算视图矩阵

    void SetProjection(float left, float right, float bottom, float top);  // 更新投影矩阵

private:
    glm::mat4 m_ProjectionMatrix;       // 投影矩阵
    glm::mat4 m_ViewMatrix;             // 视图矩阵
    glm::mat4 m_ViewProjectionMatrix;   // 投影 * 视图矩阵

    glm::vec3 m_Position = glm::vec3(0.0f); // 相机位置
    float m_Rotation = 0.0f;                // 相机旋转角度（绕 Z 轴，单位：度）
    float m_Zoom = 1.0f;                    // 相机缩放
};
