#pragma once
#include "Entity.h"
#include "glm/glm.hpp"
#include "../Shader/Shader_P2_C3.h"
#include "../Core/OrthoCamera.h"

class Triangle // : public Entity
{
public:
	Triangle()
	{
		m_vao = -1;
		m_vbo = -1;
	};
	~Triangle()
	{		
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);		
	};


	virtual void Init()  // 数据准备
	{	
	    // 1.准备Shader
		m_shader.Initialize();

		// 2.准备数据
		typedef struct Vertex
		{
			float x, y;
			float r, g, b;

		} Vertex;

		Vertex vertices[3] =
		{
			{  -1.0f, -1.0f  , 1.f, 0.f, 0.f},
			{   1.0f, -1.0f  , 0.f, 1.f, 0.f},
			{    0.f,  1.0f  , 0.f, 0.f, 1.f}
		};

		// Shader应用绑定顶点缓冲区数据
		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);
		// 创建显存并向顶点缓冲填充数据
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(m_shader.m_position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(m_shader.m_position);
		glVertexAttribPointer(m_shader.m_color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(m_shader.m_color);

	}


	virtual void Render(const OrthoCamera &camera) // 绘制
	{ 
	 
		m_shader.Begin();
		glBindVertexArray(m_vao);
		glUniformMatrix4fv(m_shader.m_mvp, 1, GL_FALSE, (const GLfloat*)&camera.GetViewProjectionMatrix());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_shader.End();
	}


public:
	unsigned        m_vao;
	unsigned        m_vbo;
	Shader_P2_C3    m_shader;

};

