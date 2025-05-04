#include "OrthoCamera.h"

OrthoCamera::OrthoCamera()
    : OrthoCamera(0.0f, 1280.0f, 0.0f, 720.0f) // 默认构造，设置视口
{
}

OrthoCamera::OrthoCamera(float left, float right, float bottom, float top)
    : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
    m_ViewMatrix(1.0f)
{
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void OrthoCamera::SetPosition(const glm::vec3& position)
{
    m_Position = position;
    RecalculateViewMatrix();
}

void OrthoCamera::SetRotation(float rotation)
{
    m_Rotation = rotation;
    RecalculateViewMatrix();
}

void OrthoCamera::SetZoom(float zoom)
{
    m_Zoom = zoom;
    RecalculateViewMatrix();
}

void OrthoCamera::RecalculateViewMatrix()
{
    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), m_Position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

    m_ViewMatrix = glm::inverse(transform);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void OrthoCamera::SetProjection(float left, float right, float bottom, float top)
{
    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    RecalculateViewMatrix();
}
