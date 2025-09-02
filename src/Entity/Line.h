#pragma once
#include "Entity.h"
#include "glm/glm.hpp"
#include "../Shader/Shader_P3_C3.h"
#include "../Core/OrthoCamera.h"
#include <vector>
 

class Line  
{
public:
	Line()
	{
		m_vao = -1;
		m_vbo = -1;
	};
	~Line()
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
		glVertexAttribPointer(m_shader.m_position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(m_shader.m_position);
		glVertexAttribPointer(m_shader.m_color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(m_shader.m_color);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		 

	} 

	void AddLine(glm::vec3 start, glm::vec3 end, glm::vec3 color) {
		m_vertices.push_back({ start.x, start.y, start.z, color.r, color.g, color.b });
		m_vertices.push_back({ end.x, end.y, end.z, color.r, color.g, color.b });

		// 更新VBO
		//glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		//glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_DYNAMIC_DRAW);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	virtual void Render(const OrthoCamera& camera) // 绘制
	{ 
		m_shader.Begin();
		glBindVertexArray(m_vao);
		glUniformMatrix4fv(m_shader.m_mvp, 1, GL_FALSE, (const GLfloat*)&camera.GetViewProjectionMatrix());
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_vertices.size()));
		m_shader.End();
		
	}


	typedef struct Vertex
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

class Line2 :public Entity
{
protected:
	glm::vec3 m_start ;
	glm::vec3 m_end;
	glm::vec3 m_color;
public:
	Line2()=default;
	Line2(glm::vec3 start, glm::vec3 end, glm::vec3 color)
	{ 
		m_start = start;
		m_end = end;
		m_color = color;
	};
	virtual ~Line2() = default;
	
	virtual   void Draw( OpenGLDC* pDc)
	{
		pDc->DrawLine(m_start, m_end, m_color);
	};
	virtual void Serialize()
	{
	};

};