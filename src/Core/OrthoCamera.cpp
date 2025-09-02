#include "OrthoCamera.h"
 
OrthoCamera::OrthoCamera()
{
}

OrthoCamera::~OrthoCamera()
{
}

void OrthoCamera::SetView(int windowWidth, int windowHeight)
{
    m_FixedHeight = windowHeight;
    m_WindowWidth = windowWidth;
    m_WindowHeight = windowHeight;
    RecalculateMatrix();
}

void OrthoCamera::SetView(float fixedHeight, int windowWidth, int windowHeight)
{
    m_FixedHeight = fixedHeight;
    m_WindowWidth = windowWidth;
    m_WindowHeight = windowHeight;
    RecalculateMatrix();
}

void OrthoCamera::SetPosition(const glm::vec3& pos)
{
    m_Position = pos;
    RecalculateMatrix();
}

void OrthoCamera::SetZoom(float zoom)
{
    m_Zoom = zoom;
    RecalculateMatrix();
}

void OrthoCamera::RecalculateMatrix()
{
    float aspect = static_cast<float>(m_WindowWidth) / m_WindowHeight;
    float halfHeight = (m_FixedHeight * 0.5f) / m_Zoom;
    float halfWidth = halfHeight * aspect;

    float left = m_Position.x - halfWidth;
    float right = m_Position.x + halfWidth;
    float bottom = m_Position.y - halfHeight;
    float top = m_Position.y + halfHeight;

    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1000.0f, 1000.0f); // 扩展 zNear/zFar 范围

    // 这里是关键：
    m_ViewMatrix = glm::translate(glm::mat4(1.0f), -m_Position); // x/y/z 都考虑进去

    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
