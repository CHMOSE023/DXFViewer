#pragma once
#include "DxfReader.h"
class  DxfReaderAscii :public DxfReader
{
public:
	DxfReaderAscii(std::ifstream* stream) :DxfReader(stream) { m_skip = true; };
	~DxfReaderAscii() {};
	virtual bool ReadString(std::string* text);
	virtual bool ReadString();
	virtual bool ReadCode(int* code) ;
	virtual bool ReadDouble(); 
};
