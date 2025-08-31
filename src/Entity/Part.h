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
		ClearEntities(); // ����ʱ��������ʵ��
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
		AddEntity(new Line2(
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		));  

		AddEntity(new Line2(
			glm::vec3(1.0f, 1.0f, 0.0f),
			glm::vec3(2.0f, 0.5f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		)); 
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
			delete* it;// �ֶ��ͷ��ڴ�
			m_entities.erase(it, m_entities.end());
		}
	}
	 
	// ��ȡʵ������
	size_t GetEntityCount() const noexcept
	{
		return m_entities.size();
	}

	// �������ʵ��
	void ClearEntities() {
		for (auto* e : m_entities)
			delete e;
		m_entities.clear();
	}

};

