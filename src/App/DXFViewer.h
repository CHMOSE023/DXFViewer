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

        //�����ʱ�����Ĵ�С
        int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
        strSrc = new WCHAR[i + 1];
        MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

        //�����ʱ�����Ĵ�С
        i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
        szRes = new CHAR[i + 1];
        WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

        result = szRes;
        delete[]strSrc;
        delete[]szRes;

        return result;
    }

    // ȥ��ǰ��ո�
    std::string trim(const std::string& str) {
        auto first = str.find_first_not_of(" \t\r\n");
        auto last = str.find_last_not_of(" \t\r\n");
        return (first == std::string::npos) ? "" : str.substr(first, last - first + 1);
    }

    // ��ȡ����-ֵ��
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
        return (it != layerColors.end()) ? it->second : glm::vec3(1.0); // Ĭ��Ϊ��ɫ
    }

    /// <summary>
    /// ��ȡֱ��
    /// </summary> 
    void ReadLine(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "�޷����ļ�: " << filename << std::endl;
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
            // ��¼��ǰͼ�㣨�����ں�����ɫ����
            if (code == "100" && value == "AcDbEntity") {
                while (ReadCodes(file, code, value)) {
                    if (code == "8") {
                        currentLayer = value;
                        break;
                    }
                }
            }

            // ���� AcDbLine
            if (code == "100" && value == "AcDbLine") {
                lineEntity.clear();

                while (ReadCodes(file, code, value)) {
                    auto it = codeToKey.find(code);
                    if (it != codeToKey.end()) {
                        lineEntity[it->second] = value;
                    }

                    if (lineEntity.size() == 6) {
                        // ��ȡ��Ϻ��ٽ��� + ���ֱ��
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

                        glm::vec3 color = GetColorByLayer(currentLayer); // ��ѡ

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

        // �����ļ�
        ReadLine(R"(.\Dxf\line.dxf)");

		printf("��ȡ��ֱ�� %d ��\r\n",(m_line.GetSize()/2));
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
        glLineWidth(2.0f); // �����߿�Ϊ 2 ����
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

