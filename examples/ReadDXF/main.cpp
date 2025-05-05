#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <Windows.h>

using Entity = std::map<std::string, std::string>;
/// <summary>
/// ifstream��getline��ȡ��������������
/// </summary> 
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

 
// ���� DXF ������ LINE ʵ���ͼ�������
std::vector<Entity> ParseDXFLines(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "�޷����ļ�: " << filename << std::endl;
        return {};
    }

    std::vector<Entity> lineEntities;
    Entity currentEntity;
    std::string code, value;
    std::string currentLayer;

    while (ReadCodes(file, code, value)) {
        // ����ÿ�� AcDbEntity ʵ��
        if (code == "100" && value == "AcDbEntity") {
            // ��ȡͼ����Ϣ
            while (ReadCodes(file, code, value)) {
                if (code == "8") {
                    currentLayer = value;  // ����ͼ����
                    break;
                }
            }
        }

        // ���� AcDbLine ʵ��
        if (code == "100" && value == "AcDbLine") {
            Entity lineEntity;
            lineEntity["Layer"] = currentLayer; // ����ͼ����Ϣ

            // ��ȡ��� (10, 20, 30)
            while (ReadCodes(file, code, value)) {
                if (code == "10") lineEntity["StartX"] = value;
                if (code == "20") lineEntity["StartY"] = value;
                if (code == "30") lineEntity["StartZ"] = value;

                // ��ȡ�յ� (11, 21, 31)
                if (code == "11") lineEntity["EndX"] = value;
                if (code == "21") lineEntity["EndY"] = value;
                if (code == "31") lineEntity["EndZ"] = value;

                // ����Ƿ�����һ���߶Σ��ɸ�����Ҫ������ȡ������Ϣ��
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
    // ���ÿ���̨���Ϊ UTF-8 
    std::string filename = R"(C:\Users\ml\Desktop\Drawing1.dxf)";
    
    // �����ļ�
    auto lines = ParseDXFLines(filename);

    // ����������߶���Ϣ
    for (const auto& entity : lines) {
        std::string layer = entity.count("Layer") ? entity.at("Layer") : "(δ֪ͼ��)";
        std::string x1 = entity.count("StartX") ? entity.at("StartX") : "?";
        std::string y1 = entity.count("StartY") ? entity.at("StartY") : "?";
        std::string x2 = entity.count("EndX") ? entity.at("EndX") : "?";
        std::string y2 = entity.count("EndY") ? entity.at("EndY") : "?";

        std::cout << "ͼ��: " << layer
            << "�����: (" << x1 << ", " << y1 << ")"
            << "���յ�: (" << x2 << ", " << y2 << ")"
            << std::endl;
    }
    return 0;
} 