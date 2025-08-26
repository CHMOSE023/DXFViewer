#pragma once
#include "glm/glm.hpp"
#include "../Shader/ShaderCircle.h"
#include "../Core/OrthoCamera.h"
#include <vector> 

class Circle {
public:
    Circle() : m_vao(0), m_vbo(0), m_ssbo(0) {
        // 生成单位圆顶点（用于 GL_LINE_LOOP）
        const int segments = 100;
        for (int i = 0; i < segments; ++i) {
            float angle = glm::radians(360.0f * i / static_cast<float>(segments));
            glm::vec3 point(cos(angle), sin(angle), 0.0f);
            m_vertices.push_back({ point.x, point.y, point.z });
        }
    }

    ~Circle() {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ssbo);
    }

    void Init() {
        m_shader.Initialize();

        // 创建 VAO + VBO（单位圆顶点）
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // 创建 SSBO
        glGenBuffers(1, &m_ssbo);
    }

    void AddCircle(const glm::vec3& center, float radius, const glm::vec3& color) {
        m_circleEntity.push_back({ center, radius, color, 0.0f });
    }

    void UpdateSSBO() {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(CircleEntity) * m_circleEntity.size(), m_circleEntity.data(), GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssbo); // binding = 0
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void Render(const OrthoCamera& camera) {
        if (m_circleEntity.empty()) return;

        UpdateSSBO();

        m_shader.Begin();
        glBindVertexArray(m_vao);

        glUniformMatrix4fv(m_shader.m_mvp, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix()));

        // Instanced draw
        glDrawArraysInstanced(GL_LINE_LOOP, 0, static_cast<GLsizei>(m_vertices.size()), static_cast<GLsizei>(m_circleEntity.size()));

        glBindVertexArray(0);
        m_shader.End();
    }

    struct CircleEntity {
        glm::vec3 center;
        float radius;
        glm::vec3 color;
        float pad; // padding to 16-byte alignment
    };

    struct Vertex {
        float x, y, z;
    };

private:
    GLuint m_vao, m_vbo, m_ssbo;
    ShaderCircle m_shader;

    std::vector<Vertex> m_vertices;
    std::vector<CircleEntity> m_circleEntity;
};
