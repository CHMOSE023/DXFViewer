#pragma once
#include "Application.h"
#include <stdio.h>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include "../Entity/Line.h" 
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <Windows.h>
#include  "../Entity/Circle.h"
#include  "../Entity/Part.h" 
#include "../Core/dx_data.h"
#include "../Core/dx_iface.h"
#include <algorithm>
#include <locale>

class DXFViewer :public Application
{ 
public:
	DXFViewer() {
		m_dc = nullptr; 
        m_part = nullptr;
    };
    ~DXFViewer() {
        delete m_dc;
    }; 

    glm::vec3 getColorFromIndex(int colorIndex)
    {    // 处理特殊颜色（ByLayer、ByBlock）
        if (colorIndex == 256) {
            // "ByLayer" - 默认返回白色（或可自定义）
            return glm::vec3(1.0f, 1.0f, 1.0f);
        }
        if (colorIndex == 257) {
            // "ByBlock" - 默认返回红色（或可自定义）
            return glm::vec3(1.0f, 0.0f, 0.0f);
        }

        // 限制索引在0-255范围内
        colorIndex = std::clamp(colorIndex, 0, 255);

        // 定义AutoCAD标准索引颜色表（0-255）
        static const std::unordered_map<int, glm::vec3> colorMap = {
            {0,  glm::vec3(0.0f, 0.0f, 0.0f)},      // 黑色
            {1,  glm::vec3(1.0f, 0.0f, 0.0f)},      // 红色
            {2,  glm::vec3(1.0f, 1.0f, 0.0f)},      // 黄色
            {3,  glm::vec3(0.0f, 1.0f, 0.0f)},      // 绿色
            {4,  glm::vec3(0.0f, 1.0f, 1.0f)},      // 青色
            {5,  glm::vec3(0.0f, 0.0f, 1.0f)},      // 蓝色
            {6,  glm::vec3(1.0f, 0.0f, 1.0f)},      // 洋红
            {7,  glm::vec3(1.0f, 1.0f, 1.0f)},      // 白色
            // 可以继续添加更多颜色...
            {8,  glm::vec3(0.5f, 0.5f, 0.5f)},      // 灰色
            {9,  glm::vec3(0.75f, 0.75f, 0.75f)},    // 浅灰
            // ... 其他颜色
            {255, glm::vec3(0.5f, 0.5f, 0.5f)}      // 默认灰色
        };

        // 查找颜色，如果找不到则返回默认颜色（白色）
        auto it = colorMap.find(colorIndex);
        if (it != colorMap.end()) {
            return it->second;
        }
        else {
            return glm::vec3(1.0f, 0.0f, 1.0f);  // 默认白色
        }
    };

    void PrintEntities(dx_ifaceBlock* block) {
        if (!block) return;

        for (auto* ent : block->ent) {
            if (!ent) continue;

            switch (ent->eType) {
            case DRW::POINT: {
               
                break;
            }
            case DRW::LINE: {
                DRW_Line* line = dynamic_cast<DRW_Line*>(ent); 

                int colorIndex =  line->color; // 索引色转RGB
                glm::vec3 color = getColorFromIndex(colorIndex);
                m_part->AddEntity(new Line(
                    glm::vec3(line->basePoint.x, line->basePoint.y, line->basePoint.z),
                    glm::vec3(line->secPoint.x, line->secPoint.y, line->secPoint.z),
                    color // int -> RGB
                ));
                break;            
            }
            case DRW::CIRCLE: {
               
                break;
            }
            case DRW::ARC: {
                
                break;
            }
            case DRW::LWPOLYLINE:
                std::cout << "Entity: LWPOLYLINE (vertices not shown)\n";
                break;
            default:
                std::cout << "Entity: UNKNOWN\n";
                break;
            }
        }
    }

	virtual void  Startup()
	{ 
        // 解析文件 
		const char* fileName = "C:\\Users\\BIM\\Desktop\\CQUD_TY_AAa中.dxf";  

		dx_data fData;
		dx_iface* input = new dx_iface();
		bool badState = input->fileImport(fileName, &fData, false);
        
        m_part = new Part();

        m_part->Serialize();
         
        PrintEntities(fData.mBlock);

        m_dc = new OpenGLDC(m_camera);

        m_dc->Initialize();


       // printf(" %d 个实体\r\n", (m_part.GetEntityCount()));
	}; 

	virtual void  Render()
	{	  
        m_part->Draw(m_dc);
	}; 

	virtual void  Shutdown()
	{
		printf("Shutdown\r\n");
	};

public:

    Part* m_part;

    OpenGLDC * m_dc;
};

