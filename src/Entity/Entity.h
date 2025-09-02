#pragma once
#include "../Shader/Shader.h" 
#include "../Core/OpenGLDC.h" 
class Entity
{
public:
	Entity() = default; 
	virtual ~Entity() = default;;  // 通过基类指针删除派生类
	virtual void Draw(OpenGLDC* pDc) = 0; // 纯虚函数继承类必须实现
	virtual	void Serialize() {}; // 序列化
};