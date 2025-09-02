#pragma once
#include "Base.h"
#include <string>
#include <memory> 

/// <summary>
/// 文本编码转换器
/// </summary>
class  TextCodec
{
public:
    TextCodec() {};
    ~TextCodec() {};

    std::string FromUtf8(const std::string& s);
    std::string ToUtf8(const std::string& s);
    void SetVersion(const std::string& v, bool dxfFormat);
    void SetVersion(Version v, bool dxfFormat);
    void SetCodePage(const std::string& c, bool dxfFormat);
    std::string GetCodePage() { return m_cp; }  
    int GetVersion() { return m_version; }

private:
    std::string CorrectCodePage(const std::string& s);

public:
    Version m_version{ UNKNOWNV };
    std::string m_cp;
    std::unique_ptr<Converter> m_conv;
};

class  Converter
{
public:
    Converter(const int* t, int l) :m_table{ t }, m_cpLength{ l }
    {
    }
    virtual ~Converter() = default;
    virtual std::string FromUtf8(const std::string& s) { return s; }
    virtual std::string ToUtf8(const std::string& s);
    std::string EncodeText(const std::string& stmp);
    std::string DecodeText(int c);
    std::string EncodeNum(int c);
    int DecodeNum(const std::string& s, int* b);

    const int* m_table{ nullptr };
    int m_cpLength;
};

class ConvUTF16 : public Converter {
public:
    ConvUTF16() :Converter(nullptr, 0) {}
    std::string FromUtf8(const std::string& s) override;
    std::string ToUtf8(const std::string& s) override;
};

class ConvTable : public Converter {
public:
    ConvTable(const int* t, int l) :Converter(t, l) {}
    std::string FromUtf8(const std::string& s) override;
    std::string ToUtf8(const std::string& s) override;
};

class  ConvDBCSTable : public Converter {
public:
    ConvDBCSTable(const int* t, const int* lt, const int dt[][2], int l)
        : Converter(t, l)
        , m_leadTable{ lt }
        , m_doubleTable{ dt }
    {
    }

    std::string FromUtf8(const std::string& s) override;
    std::string ToUtf8(const std::string& s) override;
private:
    const int* m_leadTable{ nullptr };
    const int (*m_doubleTable)[2];

};

class  Conv932Table : public Converter {
public:
    Conv932Table();
    std::string FromUtf8(const std::string& s) override;
    std::string ToUtf8(const std::string& s) override;

};