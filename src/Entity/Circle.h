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
	};
	~Circle()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
	};


	virtual void Init()  // 数据准备
	{
		// 1.准备Shader
		m_shader.Initialize();
		 
		// Shader应用绑定顶点缓冲区数据
		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);
		// 创建显存并向顶点缓冲填充数据
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

		// 原上顶点 (vec3)
		glVertexAttribPointer(m_shader.m_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(m_shader.m_position);
		 
		
		// 颜色 (vec3)	 
		glVertexAttribPointer(m_shader.m_color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(m_shader.m_color);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		  
	} 

	void AddCircle(glm::vec3 center, float radius, glm::vec3 color)
	{
		// 计算圆周上的点 360
		for (int i = 0; i < 360; ++i)
		{
			float angle = glm::radians(static_cast<float>(i));
			glm::vec3 point(center.x + radius * cos(angle), center.y + radius * sin(angle), center.z);
			m_vertices.push_back({ point.x, point.y, point.z, color.r, color.g, color.b });
		}
	}
	 

	virtual void Render(const OrthoCamera& camera) // 绘制
	{ 
		m_shader.Begin();
		glBindVertexArray(m_vao);
		glUniformMatrix4fv(m_shader.m_mvp, 1, GL_FALSE, (const GLfloat*)&camera.GetViewProjectionMatrix());
		glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(m_vertices.size()));
		m_shader.End();		
	}


	typedef struct CircleVertex
	{
		float x, y, z;
		float r, g, b;

	} Vertex;

	unsigned int GetSize()
	{
		return m_vertices.size();
	}

private:
	GLuint         m_vao;
	GLuint         m_vbo;
	Shader_P3_C3   m_shader;

	std::vector<Vertex> m_vertices;
	 

};

