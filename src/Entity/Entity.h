#pragma once
#include "../Shader/Shader.h" 
#include "../Core/OpenGLDC.h" 
class Entity
{
public:
	Entity() = default; 
	virtual ~Entity() = default;;  // ͨ������ָ��ɾ��������
	virtual void Draw(OpenGLDC* pDc) = 0; // ���麯���̳������ʵ��
	virtual	void Serialize() {}; // ���л�
};