#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/// <summary>
/// ʵ��һ������ 2D ��Ⱦ����������ࡣ
/// �ṩ�����λ�á���ת��ͶӰ�������ͼ����Ȼ������ܡ�
/// </summary>
class OrthoCamera
{
public:
    OrthoCamera(); // Ĭ�Ϲ��캯��
    OrthoCamera(float left, float right, float bottom, float top); // �вι��캯��

    void SetPosition(const glm::vec3& position);
    void SetRotation(float rotation);
    void SetZoom(float zoom);  // ��������
    float GetZoom() const { return m_Zoom; }

    const glm::vec3& GetPosition() const { return m_Position; }
    float GetRotation() const { return m_Rotation; }

    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    void RecalculateViewMatrix(); // ������ͼ����

    void SetProjection(float left, float right, float bottom, float top);  // ����ͶӰ����

private:
    glm::mat4 m_ProjectionMatrix;       // ͶӰ����
    glm::mat4 m_ViewMatrix;             // ��ͼ����
    glm::mat4 m_ViewProjectionMatrix;   // ͶӰ * ��ͼ����

    glm::vec3 m_Position = glm::vec3(0.0f); // ���λ��
    float m_Rotation = 0.0f;                // �����ת�Ƕȣ��� Z �ᣬ��λ���ȣ�
    float m_Zoom = 1.0f;                    // �������
};
