#pragma once
#include "Entity.h"
#include <vector>
#include <memory>
#include "Line.h"
class Part : public Entity
{
protected:
	std::vector<Entity*> m_entities;

public:
	Part() = default;
	~Part()override {
		ClearEntities(); // 析构时清理所有实体
	};  

public:
	virtual void Draw(OpenGLDC* pDc)
	{
		for (auto& e : m_entities)
		{
			e->Draw(pDc);
		}
	};

	virtual void Serialize() 
	{  
	};

	void AddEntity(Entity* entity)
	{
		m_entities.push_back(entity);
	}

	void RemoveEntity(Entity* entity)
	{
		auto it = std::remove(m_entities.begin(), m_entities.end(), entity);
		if (it!=m_entities.end())
		{
			delete* it;// 手动释放内存
			m_entities.erase(it, m_entities.end());
		}
	}
	 
	// 获取实体数量
	size_t GetEntityCount() const noexcept
	{
		return m_entities.size();
	}

	// 清空所有实体
	void ClearEntities() {
		for (auto* e : m_entities)
			delete e;
		m_entities.clear();
	}

};

