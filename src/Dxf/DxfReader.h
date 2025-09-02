#pragma once
#include <fstream>
#include "TextCodec.h"
class DxfReader
{
public:
	enum TYPE {
		STRING,
		INT32,
		INT64,
		DOUBLE,
		BOOL,
		INVALID
	};
	enum TYPE m_type;

public:
	DxfReader(std::ifstream* stream) {
		m_filestr = stream;
		m_type = INVALID;
	};
	~DxfReader() {};   
	bool ReadRec(int* code);
	std::string GetString() { return m_strData; }

	int getHandleString();//Convert hex string to int
	std::string ToUtf8String(std::string t) { return m_decoder.ToUtf8(t); }
	std::string GetUtf8String() { return m_decoder.ToUtf8(m_strData); }

	int GetVersion() { return m_decoder.GetVersion(); };
	void SetIgnoreComments(const bool bValue) { m_bIgnoreComments = bValue; }
  
protected:
	virtual bool ReadCode(int* code) = 0; //return true if successful (not EOF)
	virtual bool ReadString(std::string* text) = 0;
	virtual bool ReadString() = 0;
	virtual bool ReadDouble() = 0; 

protected:
	std::ifstream*  m_filestr;
	std::string     m_strData;
	double          m_doubleData;
	signed int      m_intData;

	unsigned long long int m_int64;

	bool  m_skip; // true for ascii dxf, false for binary
private:
	TextCodec  m_decoder;
	bool       m_bIgnoreComments{ false };
};
