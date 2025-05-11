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

class DXFViewer :public Application
{ 

public:	  
    std::string UTF8ToGB(const char* str)
    {
        std::string result;
        WCHAR* strSrc;
        LPSTR szRes;

        //获得临时变量的大小
        int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
        strSrc = new WCHAR[i + 1];
        MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

        //获得临时变量的大小
        i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
        szRes = new CHAR[i + 1];
        WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

        result = szRes;
        delete[]strSrc;
        delete[]szRes;

        return result;
    }

    // 去除前后空格
    std::string trim(const std::string& str) {
        auto first = str.find_first_not_of(" \t\r\n");
        auto last = str.find_last_not_of(" \t\r\n");
        return (first == std::string::npos) ? "" : str.substr(first, last - first + 1);
    }

    // 读取组码-值对
    bool ReadCodes(std::ifstream& file, std::string& code, std::string& value) {
        if (!std::getline(file, code)) return false;
        if (!std::getline(file, value)) return false;

        std::string strCode = UTF8ToGB(code.c_str()).c_str();
        std::string strValue = UTF8ToGB(value.c_str()).c_str();
        code = trim(strCode);
        value = trim(strValue);
        return true;
    }   

    glm::vec3 GetColorByLayer(const std::string& layer)
    {
        static std::map<std::string, glm::vec3> layerColors = {
            { "0",     {1.0f, 1.0f, 1.0f} },
            { "WALL",  {1.0f, 0.0f, 0.0f} },
            { "DOOR",  {0.0f, 1.0f, 0.0f} },
            { "WINDOW",{0.0f, 0.5f, 1.0f} }
        };

        auto it = layerColors.find(layer);
        return (it != layerColors.end()) ? it->second : glm::vec3(1.0); // 默认为白色
    }

    /// <summary>
    /// 读取直线
    /// </summary> 
    void ReadLine(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "无法打开文件: " << filename << std::endl;
            return;
        }

        std::string code, value;
        std::string currentLayer;

        std::map<std::string, std::string> lineEntity;

        const std::map<std::string, std::string> codeToKey = {
            { "10", "StartX" }, { "20", "StartY" }, { "30", "StartZ" },
            { "11", "EndX"   }, { "21", "EndY"   }, { "31", "EndZ"   }
        };

        while (ReadCodes(file, code, value)) {
            // 记录当前图层（可用于后续颜色处理）
            if (code == "100" && value == "AcDbEntity") {
                while (ReadCodes(file, code, value)) {
                    if (code == "8") {
                        currentLayer = value;
                        break;
                    }
                }
            }

            // 解析 AcDbLine
            if (code == "100" && value == "AcDbLine") {
                lineEntity.clear();

                while (ReadCodes(file, code, value)) {
                    auto it = codeToKey.find(code);
                    if (it != codeToKey.end()) {
                        lineEntity[it->second] = value;
                    }

                    if (lineEntity.size() == 6) {
                        // 读取完毕后再解析 + 添加直线
                        glm::vec3 start(
                            std::stof(lineEntity["StartX"]),
                            std::stof(lineEntity["StartY"]),
                            std::stof(lineEntity["StartZ"])
                        );
                        glm::vec3 end(
                            std::stof(lineEntity["EndX"]),
                            std::stof(lineEntity["EndY"]),
                            std::stof(lineEntity["EndZ"])
                        );

                        glm::vec3 color = GetColorByLayer(currentLayer); // 可选

                        m_line.AddLine(start, end, color);
                        break;
                    }
                }
            }
        }

        file.close();
    }

	virtual void  Startup()
	{           

        // 解析文件
        ReadLine(R"(.\Dxf\line.dxf)");

		printf("读取了直线 %d 条\r\n",(m_line.GetSize()/2));
		m_line.Init();

		m_circle.Init();
		m_circle.AddCircle(glm::vec3(0, 0, 0), 5.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		m_circle.AddCircle(glm::vec3(0, 0, 0), 4.0f, glm::vec3(1.0f, 1.0f, 0.0f));
		m_circle.AddCircle(glm::vec3(0, 0, 0), 3.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		m_circle.AddCircle(glm::vec3(0, 0, 0), 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		m_circle.AddCircle(glm::vec3(0, 0, 0), 1.0f, glm::vec3(0.0f, 1.0f, 1.0f));
		 
	}; 

	virtual void  Render()
	{		  
		// m_line.Render(m_camera);
        glLineWidth(2.0f); // 设置线宽为 2 像素
		m_circle.Render(m_camera);
       
	}; 
	virtual void  Shutdown()
	{
		printf("Shutdown\r\n");
	};

private:
    Line  m_line;
    Circle m_circle;

};

