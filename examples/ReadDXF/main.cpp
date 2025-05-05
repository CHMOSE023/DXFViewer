#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <Windows.h>

using Entity = std::map<std::string, std::string>;
/// <summary>
/// ifstream和getline获取中文乱码解决方案
/// </summary> 
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

 
// 解析 DXF 中所有 LINE 实体的图层和坐标
std::vector<Entity> ParseDXFLines(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return {};
    }

    std::vector<Entity> lineEntities;
    Entity currentEntity;
    std::string code, value;
    std::string currentLayer;

    while (ReadCodes(file, code, value)) {
        // 处理每个 AcDbEntity 实体
        if (code == "100" && value == "AcDbEntity") {
            // 获取图层信息
            while (ReadCodes(file, code, value)) {
                if (code == "8") {
                    currentLayer = value;  // 保存图层名
                    break;
                }
            }
        }

        // 处理 AcDbLine 实体
        if (code == "100" && value == "AcDbLine") {
            Entity lineEntity;
            lineEntity["Layer"] = currentLayer; // 设置图层信息

            // 获取起点 (10, 20, 30)
            while (ReadCodes(file, code, value)) {
                if (code == "10") lineEntity["StartX"] = value;
                if (code == "20") lineEntity["StartY"] = value;
                if (code == "30") lineEntity["StartZ"] = value;

                // 获取终点 (11, 21, 31)
                if (code == "11") lineEntity["EndX"] = value;
                if (code == "21") lineEntity["EndY"] = value;
                if (code == "31") lineEntity["EndZ"] = value;

                // 检查是否处理完一个线段（可根据需要继续读取更多信息）
                if (lineEntity.size() == 6) {
                    lineEntities.push_back(lineEntity);
                    break;
                }
            }
        }
    }

    file.close();
    return lineEntities;
}


int main()
{
    // 设置控制台输出为 UTF-8 
    std::string filename = R"(C:\Users\ml\Desktop\Drawing1.dxf)";
    
    // 解析文件
    auto lines = ParseDXFLines(filename);

    // 输出解析的线段信息
    for (const auto& entity : lines) {
        std::string layer = entity.count("Layer") ? entity.at("Layer") : "(未知图层)";
        std::string x1 = entity.count("StartX") ? entity.at("StartX") : "?";
        std::string y1 = entity.count("StartY") ? entity.at("StartY") : "?";
        std::string x2 = entity.count("EndX") ? entity.at("EndX") : "?";
        std::string y2 = entity.count("EndY") ? entity.at("EndY") : "?";

        std::cout << "图层: " << layer
            << "，起点: (" << x1 << ", " << y1 << ")"
            << "，终点: (" << x2 << ", " << y2 << ")"
            << std::endl;
    }
    return 0;
} 