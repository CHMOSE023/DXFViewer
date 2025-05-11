#pragma once
#include "Entity.h"
#include "glm/glm.hpp"
#include "../Shader/ShaderCircle.h"
#include "../Core/OrthoCamera.h"
#include <vector>
 

class Circle
{
public:
	Circle()
	{
		m_vao = -1;
		m_vbo = -1;	 
		// ����Բ���ϵĵ� 360
		for (int i = 0; i < 360; ++i)
		{
			float angle = glm::radians(static_cast<float>(i));
			glm::vec3 point( cos(angle),sin(angle), 0);
			m_vertices.push_back({ point.x, point.y, point.z});
		}
	};
	~Circle()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
	};


	virtual void Init()  // ����׼��
	{
		// 1.׼��Shader
		m_shader.Initialize();
		 
		// ShaderӦ�ð󶨶��㻺��������
		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);
		// �����Դ沢�򶥵㻺���������
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

		// ԭ�϶��� (vec3)
		glVertexAttribPointer(m_shader.m_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(m_shader.m_position); 

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		  
	} 

	void AddCircle(glm::vec3 center, float radius, glm::vec3 color)
	{
		m_circleEntity.push_back({ center ,radius ,color });		 
	}
	 

	virtual void Render(const OrthoCamera& camera) // ����
	{ 
		m_shader.Begin();
		glBindVertexArray(m_vao);
		glUniformMatrix4fv(m_shader.m_mvp, 1, GL_FALSE, (const GLfloat*)&camera.GetViewProjectionMatrix());

		for (size_t i = 0; i < m_circleEntity.size(); i++)
		{
			const auto& entity = m_circleEntity[i];
			glUniform3fv(m_shader.m_center, 1, glm::value_ptr(entity.center));
			glUniform1f(m_shader.m_radius, entity.radius);
			glUniform3fv(m_shader.m_color, 1, glm::value_ptr(entity.color));
			glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(m_vertices.size()));
		}

		m_shader.End();		
		
	}

	typedef struct CircleEntity
	{
		glm::vec3 center; // Բ��
		float radius;    // �뾶
		glm::vec3 color; // ��ɫ
	} CircleEntity;

	typedef struct CircleVertex
	{
		float x, y, z; 
	} Vertex;	 

private:
	GLuint         m_vao;
	GLuint         m_vbo;
	ShaderCircle   m_shader;

	std::vector<Vertex> m_vertices;  
	std::vector<CircleEntity> m_circleEntity;

};

