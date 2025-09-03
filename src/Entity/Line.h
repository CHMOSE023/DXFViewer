#pragma once
#include "Entity.h"
#include "glm/glm.hpp"
#include "../Shader/Shader_P3_C3.h"
#include "../Core/OrthoCamera.h"
#include <vector>
 
class Line :public Entity
{
protected:
	glm::vec3 m_start ;
	glm::vec3 m_end;
	glm::vec3 m_color;
public:
	Line()=default;
	Line(glm::vec3 start, glm::vec3 end, glm::vec3 color)
	{ 
		m_start = start;
		m_end = end;
		m_color = color;
	};
	virtual ~Line() = default;
	
	virtual   void Draw( OpenGLDC* pDc)
	{
		pDc->DrawLine(m_start, m_end, m_color);
	};
	virtual void Serialize()
	{
	};

};